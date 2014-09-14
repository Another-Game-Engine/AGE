#include <Core/PrepareRenderThread.hpp>
#include <Core/PrepareRenderThreadCommand.hpp>
#include <Core/AScene.hh>
#include <Utils/Frustum.hpp>
#include <Core/RenderThread.hpp>
#include <Utils/ThreadQueueCommands.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Core/Drawable.hh>

namespace AGE
{
	PrepareRenderThread::PrepareRenderThread()
	{
	}

	PrepareRenderThread::~PrepareRenderThread(void)
	{
		_commandQueue.emplace<TMQ::CloseQueue>();
		_commandQueue.releaseReadability();
	}

	bool PrepareRenderThread::_init()
	{
		_octreeDrawList = AGE::Vector<DrawableCollection>();
		return true;
	}

	bool PrepareRenderThread::_initInNewThread()
	{
		return true;
	}

	bool PrepareRenderThread::_release()
	{
		return true;
	}

	bool PrepareRenderThread::_releaseInNewThread()
	{
		return true;
	}

	PrepareKey PrepareRenderThread::addMesh()
	{
		PrepareKey res;
		res.type = PrepareKey::Type::Drawable;
		if (!_freeMeshs.empty())
		{
			res.id = _freeMeshs.front();
			_freeMeshs.pop();
		}
		else
			res.id = PrepareKey::OctreeObjectId(_MeshCounter++);
		_commandQueue.emplace<PRTC::CreateDrawable>(res);
		return res;
	}

	PrepareKey PrepareRenderThread::addCamera()
	{
		PrepareKey res;
		res.type = PrepareKey::Type::Camera;
		if (!_freeCameras.empty())
		{
			res.id = _freeCameras.front();
			_freeCameras.pop();
		}
		else
			res.id = PrepareKey::OctreeObjectId(_cameraCounter++);
		_commandQueue.emplace<PRTC::CreateCamera>(res);

		return res;
	}

	PrepareKey PrepareRenderThread::addPointLight()
	{
		PrepareKey res;
		res.type = PrepareKey::Type::PointLight;
		if (!_freePointLights.empty())
		{
			res.id = _freePointLights.front();
			_freePointLights.pop();
		}
		else
			res.id = PrepareKey::OctreeObjectId(_pointLightCounter++);
		_commandQueue.emplace<PRTC::CreatePointLight>(res);
		return res;
	}

	PrepareRenderThread &PrepareRenderThread::removeElement(const PrepareKey &key)
	{
		assert(!key.invalid());
		switch (key.type)
		{
		case PrepareKey::Type::Camera:
			_freeCameras.push(key.id);
			_commandQueue.emplace<PRTC::DeleteCamera>(key);
			break;
		case PrepareKey::Type::Drawable:
			_freeMeshs.push(key.id);
			_commandQueue.emplace<PRTC::DeleteDrawable>(key);
			break;
		case PrepareKey::Type::PointLight:
			_freePointLights.push(key.id);
			_commandQueue.emplace<PRTC::DeletePointLight>(key);
			break;
		default:
			break;
		}
		return (*this);
	}

	PrepareRenderThread &PrepareRenderThread::setPointLight(float power, float range, glm::vec3 const &color, glm::vec4 const &position, const PrepareKey &id)
	{
		_commandQueue.emplace<PRTC::SetPointLight>(power, range, color, position, id);
		return (*this);
	}

	PrepareRenderThread &PrepareRenderThread::setPosition(const glm::vec3 &v, const PrepareKey &id)
	{
		_commandQueue.emplace<PRTC::Position>(id, v);
		return (*this);
	}
	PrepareRenderThread &PrepareRenderThread::setOrientation(const glm::quat &v, const PrepareKey &id)
	{
		_commandQueue.emplace<PRTC::Orientation>(id, v);
		return (*this);
	}

	PrepareRenderThread &PrepareRenderThread::setScale(const glm::vec3 &v, const PrepareKey &id)
	{
		_commandQueue.emplace<PRTC::Scale>(id, v);
		return (*this);
	}

	PrepareRenderThread &PrepareRenderThread::setCameraInfos(const PrepareKey &id
		, const glm::mat4 &projection)
	{
		_commandQueue.emplace<PRTC::CameraInfos>(id, projection);
		return (*this);
	}

	PrepareRenderThread &PrepareRenderThread::setPosition(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setPosition(v, e);
		return (*this);
	}

	PrepareRenderThread &PrepareRenderThread::setOrientation(const glm::quat &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setOrientation(v, e);
		return (*this);
	}

	PrepareRenderThread &PrepareRenderThread::setScale(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setScale(v, e);
		return (*this);
	}

	PrepareRenderThread &PrepareRenderThread::updateGeometry(const PrepareKey &key
		, const AGE::Vector<AGE::SubMeshInstance> &meshs
		, const AGE::Vector<AGE::MaterialInstance> &materials)
	{
		assert(!key.invalid() || key.type != PrepareKey::Type::Drawable);
		_commandQueue.emplace<PRTC::Geometry>(key, meshs, materials);
		return (*this);
	}

	DRAWABLE_ID PrepareRenderThread::addDrawable(USER_OBJECT_ID uid)
	{
		DRAWABLE_ID res = DRAWABLE_ID(-1);
		Drawable *co = nullptr;
		if (!_freeDrawables.empty())
		{
			res = _freeDrawables.front();
			_freeDrawables.pop();
			co = &(_drawables[res]);
		}
		else
		{
			res = _drawables.size();
			_drawables.emplace_back(Drawable());
			co = &(_drawables.back());
		}
		co->id = res;
		co->active = true;
		return res;
	}

	void PrepareRenderThread::removeDrawableObject(DRAWABLE_ID id)
	{
		_freeDrawables.push(PrepareKey::OctreeObjectId(id));
		_drawables[id].active = false;
		assert(id != (std::size_t)(-1));
	}


	bool PrepareRenderThread::_update()
	{
		_commandQueue.getDispatcher()
			.handle<PRTC::CameraInfos>([&](const PRTC::CameraInfos& msg)
		{
			Camera *co = nullptr;
			co = &_cameras[msg.key.id];
			co->hasMoved = true;
			co->projection = msg.projection;
		})
			.handle<PRTC::CreateCamera>([&](const PRTC::CreateCamera& msg)
		{
			Camera *co = nullptr;
			if (msg.key.id >= _cameras.size())
			{
				_cameras.push_back(Camera());
				co = &_cameras.back();
			}
			else
				co = &_cameras[msg.key.id];
			co->key.id = msg.key.id;
			co->active = true;
		})
			.handle<PRTC::CreatePointLight>([&](const PRTC::CreatePointLight &msg)
		{
			PointLight *co = nullptr;
			if (msg.key.id >= _pointLights.size())
			{
				_pointLights.push_back(PointLight());
				co = &_pointLights.back();
			}
			else
				co = &_pointLights.back();
			co->key.id = msg.key.id;
		})
			.handle<PRTC::CreateDrawable>([&](const PRTC::CreateDrawable& msg)
		{
			Mesh *uo = nullptr;
			if (msg.key.id >= _meshs.size())
			{
				_meshs.push_back(Mesh());
				uo = &_meshs.back();
			}
			else
				uo = &_meshs[msg.key.id];
		})
			.handle<PRTC::SetPointLight>([&](const PRTC::SetPointLight &msg)
		{
			PointLight *l = nullptr;
			l = &_pointLights[msg.key.id];
			l->color = msg.color;
			l->position = msg.position;
			l->power = msg.power;
			l->range = msg.range;
		})
			.handle<PRTC::DeleteCamera>([&](const PRTC::DeleteCamera& msg)
		{
			Camera *co = nullptr;
			co = &_cameras[msg.key.id];
			co->active = false;
		})
			.handle<PRTC::DeletePointLight>([&](const PRTC::DeletePointLight &msg)
		{
			PointLight *co = nullptr;
			co = &_pointLights[msg.key.id];
		})
			.handle<PRTC::DeleteDrawable>([&](const PRTC::DeleteDrawable& msg)
		{
			Mesh *uo = nullptr;
			uo = &this->_meshs[msg.key.id];
			for (auto &e : uo->drawableCollection)
				removeDrawableObject(e);
			uo->drawableCollection.clear();
			uo->active = false;
		})
			.handle<PRTC::Geometry>([this](const PRTC::Geometry& msg)
		{
			Mesh *uo = nullptr;
			uo = &_meshs[msg.key.id];
			for (auto &e : uo->drawableCollection)
				removeDrawableObject(e);
			uo->drawableCollection.clear();
			for (std::size_t i = 0; i < msg.submeshInstances.size(); ++i)
			{
				auto id = addDrawable(msg.key.id);
				uo->drawableCollection.push_back(id);
				_drawables[id].mesh = msg.submeshInstances[i];
				_drawables[id].material = msg.materialInstances[i];
				_drawables[id].position = uo->position;
				_drawables[id].orientation = uo->orientation;
				_drawables[id].scale = uo->scale;
			}
		})
			.handle<PRTC::Position>([&](const PRTC::Position& msg)
		{
			Camera *co = nullptr;
			Mesh *uo = nullptr;
			PointLight *l = nullptr;

			switch (msg.key.type)
			{

			case(PrepareKey::Type::Camera) :
				co = &_cameras[msg.key.id];
				co->position = msg.position;
				co->hasMoved = true;
				break;
			case(PrepareKey::Type::Drawable) :
				uo = &_meshs[msg.key.id];
				uo->position = msg.position;
				for (auto &e : uo->drawableCollection)
				{
					_drawables[e].position = uo->position;
					_drawables[e].hasMoved = true;
				}
				break;
			case(PrepareKey::Type::PointLight) :
				l = &_pointLights[msg.key.id];
				l->position = glm::vec4(msg.position.x, msg.position.y, msg.position.z, 1.0f);
				break;
			default:
				break;
			}
		})
			.handle<PRTC::Scale>([&](const PRTC::Scale& msg)
		{
			Mesh *uo = nullptr;
			Camera *co = nullptr;
			switch (msg.key.type)
			{
			case(PrepareKey::Type::Camera) :
				co = &_cameras[msg.key.id];
				co->scale = msg.scale;
				co->hasMoved = true;
				break;
			case(PrepareKey::Type::Drawable) :
				uo = &_meshs[msg.key.id];
				uo->scale = msg.scale;
				for (auto &e : uo->drawableCollection)
				{
					_drawables[e].scale = uo->scale;
					_drawables[e].hasMoved = true;
				}
				break;
			default:
				break;
			}
		})
			.handle<PRTC::Orientation>([&](const PRTC::Orientation& msg)
		{
			Mesh *uo = nullptr;
			Camera *co = nullptr;
			switch (msg.key.type)
			{
			case(PrepareKey::Type::Camera) :
				co = &_cameras[msg.key.id];
				co->orientation = msg.orientation;
				co->hasMoved = true;
				break;
			case(PrepareKey::Type::Drawable) :
				uo = &_meshs[msg.key.id];
				uo->orientation = msg.orientation;
				for (auto &e : uo->drawableCollection)
				{
					_drawables[e].orientation = uo->orientation;
					_drawables[e].hasMoved = true;
				}
				break;
			default:
				break;
			}
		})
			.handle<TMQ::CloseQueue>([&](const TMQ::CloseQueue& msg)
		{
			_isRunning = false;
			return false;
		}).handle<PRTC::PrepareDrawLists>([&](PRTC::PrepareDrawLists& msg)
		{
			static std::size_t cameraCounter = 0; cameraCounter = 0;

			for (auto &camera : _cameras)
			{
				if (!camera.active)
					continue;
				Frustum frustum;
				auto transformation = glm::scale(glm::translate(glm::mat4(1), camera.position) * glm::toMat4(camera.orientation), camera.scale);
				frustum.setMatrix(camera.projection * transformation, true);

				_octreeDrawList.emplace_back();
				for (size_t index = 0; index < _pointLights.size(); ++index)
				{
					auto &p = _pointLights[index];
					_octreeDrawList.back().lights.push_back(PointLight(p.power, p.range, p.color, p.position));
				}
				auto &drawList = _octreeDrawList.back();

				drawList.drawables.clear();

				drawList.transformation = transformation;
				drawList.projection = camera.projection;

				std::size_t drawed = 0; std::size_t total = 0;

				for (auto &e : _drawables)
				{
					if (e.active)
						++total;
					else
						continue;
					if (frustum.pointIn(e.position) == true)
					{
						if (e.hasMoved)
						{
							e.transformation = glm::scale(glm::translate(glm::mat4(1), e.position) * glm::toMat4(e.orientation), e.scale);
							e.hasMoved = false;
						}
						drawList.drawables.emplace_back(e.mesh, e.material, e.transformation);
						++drawed;
					}
				}
				++cameraCounter;
			}

			auto renderThread = getDependencyManager().lock()->getInstance<AGE::Threads::Render>();
			for (auto &e : this->_octreeDrawList)
			{
				renderThread->getCommandQueue().safeEmplace<TQC::VoidFunction>([=](){
					msg.function(e);
				});
			}
			_octreeDrawList.clear();
			renderThread->getCommandQueue().safeEmplace<RendCtxCommand::Flush>();
			renderThread->getCommandQueue().releaseReadability();

			//msg.result.set_value(std::move(_octreeDrawList));
			//_octreeDrawList.clear();
		});

		return true;
	}

}