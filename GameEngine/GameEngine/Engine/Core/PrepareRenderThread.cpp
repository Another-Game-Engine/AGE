#include "PrepareRenderThread.hpp"
#include <Core/AScene.hh>
#include <Components/MeshRenderer.hh>
#include <Core/EntityFilter.hpp>
#include <Components/CameraComponent.hpp>
#include <Utils/Frustum.hpp>
#include <chrono>
#include <Utils/DependenciesInjector.hpp>
#include <Core/RenderThread.hpp>
#include <Utils/ThreadQueueCommands.hpp>
#include <glm/gtx/quaternion.hpp>
#include "PrepareRenderThreadCommand.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


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

	const PrepareKey &PrepareRenderThread::addCullableElement()
	{
		PrepareKey res;
		res.type = PrepareKey::Type::Cullable;
		if (!_freeUserObjects.empty())
		{
			res.id = _freeUserObjects.front();
			_freeUserObjects.pop();
		}
		else
		{
			res.id = PrepareKey::OctreeObjectId(_userObjectCounter++);
		}

		_commandQueue.emplace<PRTC::CreateDrawable>(res);
		return res;
	}

	void PrepareRenderThread::removeElement(const PrepareKey &key)
	{
		assert(!key.invalid());
		switch (key.type)
		{
		case(PrepareKey::Type::Camera) :
		{
			_freeCameraObjects.push(key.id);
			_commandQueue.emplace<PRTC::DeleteCamera>(key);
		}
									  break;
		case(PrepareKey::Type::Cullable) :
		{
			_freeUserObjects.push(key.id);
			_commandQueue.emplace<PRTC::DeleteDrawable>(key);
		}
										break;
		default:
			break;
		}
	}

	const PrepareKey &PrepareRenderThread::addCameraElement()
	{
		PrepareKey res;
		res.type = PrepareKey::Type::Camera;
		if (!_freeCameraObjects.empty())
		{
			res.id = _freeCameraObjects.front();
			_freeCameraObjects.pop();
		}
		else
		{
			res.id = PrepareKey::OctreeObjectId(_cameraCounter++);
		}
		_commandQueue.emplace<PRTC::CreateCamera>(res);

		return res;
	}

	void PrepareRenderThread::setPosition(const glm::vec3 &v, const PrepareKey &id)
	{
		_commandQueue.emplace<PRTC::Position>(id, v);
	}
	void PrepareRenderThread::setOrientation(const glm::quat &v, const PrepareKey &id)
	{
		_commandQueue.emplace<PRTC::Orientation>(id, v);
	}

	void PrepareRenderThread::setScale(const glm::vec3 &v, const PrepareKey &id)
	{
		_commandQueue.emplace<PRTC::Scale>(id, v);
	}

	void PrepareRenderThread::setCameraInfos(const PrepareKey &id
		, const glm::mat4 &projection)
	{
		_commandQueue.emplace<PRTC::CameraInfos>(id, projection);
	}

	void PrepareRenderThread::setPosition(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setPosition(v, e);
	}

	void PrepareRenderThread::setOrientation(const glm::quat &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setOrientation(v, e);
	}

	void PrepareRenderThread::setScale(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setScale(v, e);
	}

	void PrepareRenderThread::updateGeometry(const PrepareKey &key
		, const AGE::Vector<AGE::SubMeshInstance> &meshs
		, const AGE::Vector<AGE::MaterialInstance> &materials)
	{
		assert(!key.invalid() || key.type != PrepareKey::Type::Cullable);
		_commandQueue.emplace<PRTC::Geometry>(key, meshs, materials);
	}

	//-----------------------------------------------------------------

	PrepareRenderThread::DRAWABLE_ID PrepareRenderThread::addDrawableObject(PrepareRenderThread::USER_OBJECT_ID uid)
	{
		DRAWABLE_ID res = DRAWABLE_ID(-1);
		CullableObject *co = nullptr;
		if (!_freeCullableObjects.empty())
		{
			res = _freeCullableObjects.front();
			_freeCullableObjects.pop();
			co = &(_cullableObjects[res]);
		}
		else
		{
			res = _cullableObjects.size();
			_cullableObjects.emplace_back(CullableObject());
			co = &(_cullableObjects.back());
		}
		co->id = res;
		co->active = true;
		return res;
	}

	void PrepareRenderThread::removeDrawableObject(DRAWABLE_ID id)
	{
		_freeCullableObjects.push(PrepareKey::OctreeObjectId(id));
		_cullableObjects[id].active = false;
		assert(id != (std::size_t)(-1));
	}


	bool PrepareRenderThread::_update()
	{
		_commandQueue.getDispatcher()
			.handle<PRTC::CameraInfos>([&](const PRTC::CameraInfos& msg)
		{
			CameraObject *co = nullptr;
			co = &_cameraObjects[msg.key.id];
			co->hasMoved = true;
			co->projection = msg.projection;
		})
			.handle<PRTC::CreateCamera>([&](const PRTC::CreateCamera& msg)
		{
			CameraObject *co = nullptr;
			if (msg.key.id >= _cameraObjects.size())
			{
				_cameraObjects.push_back(CameraObject());
				co = &_cameraObjects.back();
			}
			else
			{
				co = &_cameraObjects[msg.key.id];
			}
			co->key.id = msg.key.id;
			co->active = true;
		})
			.handle<PRTC::CreateDrawable>([&](const PRTC::CreateDrawable& msg)
		{
			UserObject *uo = nullptr;
			if (msg.key.id >= _userObjects.size())
			{
				_userObjects.push_back(UserObject());
				uo = &_userObjects.back();
			}
			else
			{
				uo = &_userObjects[msg.key.id];
			}
		})
			.handle<PRTC::DeleteCamera>([&](const PRTC::DeleteCamera& msg)
		{
			CameraObject *co = nullptr;
			co = &_cameraObjects[msg.key.id];
			co->active = false;
		})
			.handle<PRTC::DeleteDrawable>([&](const PRTC::DeleteDrawable& msg)
		{
			UserObject *uo = nullptr;
			uo = &this->_userObjects[msg.key.id];
			for (auto &e : uo->drawableCollection)
			{
				removeDrawableObject(e);
			}
			uo->drawableCollection.clear();
			uo->active = false;
		})
			.handle<PRTC::Geometry>([this](const PRTC::Geometry& msg)
		{
			UserObject *uo = nullptr;
			uo = &_userObjects[msg.key.id];
			//assert(uo->active == true);
			for (auto &e : uo->drawableCollection)
			{
				removeDrawableObject(e);
			}
			uo->drawableCollection.clear();
			for (std::size_t i = 0; i < msg.submeshInstances.size(); ++i)
			{
				auto id = addDrawableObject(msg.key.id);
				uo->drawableCollection.push_back(id);
				_cullableObjects[id].mesh = msg.submeshInstances[i];
				_cullableObjects[id].material = msg.materialInstances[i];
				_cullableObjects[id].position = uo->position;
				_cullableObjects[id].orientation = uo->orientation;
				_cullableObjects[id].scale = uo->scale;
			}
		})
			.handle<PRTC::Position>([&](const PRTC::Position& msg)
		{
			UserObject *uo = nullptr;
			CameraObject *co = nullptr;
			switch (msg.key.type)
			{
			case(PrepareKey::Type::Camera) :
				co = &_cameraObjects[msg.key.id];
				co->position = msg.position;
				co->hasMoved = true;
				break;
			case(PrepareKey::Type::Cullable) :
				uo = &_userObjects[msg.key.id];
				uo->position = msg.position;
				for (auto &e : uo->drawableCollection)
				{
					_cullableObjects[e].position = uo->position;
					_cullableObjects[e].hasMoved = true;
				}
				break;
			default:
				break;
			}
		})
			.handle<PRTC::Scale>([&](const PRTC::Scale& msg)
		{
			UserObject *uo = nullptr;
			CameraObject *co = nullptr;
			switch (msg.key.type)
			{
			case(PrepareKey::Type::Camera) :
				co = &_cameraObjects[msg.key.id];
				co->scale = msg.scale;
				co->hasMoved = true;
				break;
			case(PrepareKey::Type::Cullable) :
				uo = &_userObjects[msg.key.id];
				uo->scale = msg.scale;
				for (auto &e : uo->drawableCollection)
				{
					_cullableObjects[e].scale = uo->scale;
					_cullableObjects[e].hasMoved = true;
				}
				break;
			default:
				break;
			}
		})
			.handle<PRTC::Orientation>([&](const PRTC::Orientation& msg)
		{
			UserObject *uo = nullptr;
			CameraObject *co = nullptr;
			switch (msg.key.type)
			{
			case(PrepareKey::Type::Camera) :
				co = &_cameraObjects[msg.key.id];
				co->orientation = msg.orientation;
				co->hasMoved = true;
				break;
			case(PrepareKey::Type::Cullable) :
				uo = &_userObjects[msg.key.id];
				uo->orientation = msg.orientation;
				for (auto &e : uo->drawableCollection)
				{
					_cullableObjects[e].orientation = uo->orientation;
					_cullableObjects[e].hasMoved = true;
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

			for (auto &camera : _cameraObjects)
			{
				if (!camera.active)
					continue;
				Frustum frustum;
				auto transformation = glm::scale(glm::translate(glm::mat4(1), camera.position) * glm::toMat4(camera.orientation), camera.scale);
				frustum.setMatrix(camera.projection * transformation, true);

				_octreeDrawList.emplace_back();
				auto &drawList = _octreeDrawList.back();

				drawList.drawables.clear();

				drawList.transformation = transformation;
				drawList.projection = camera.projection;

				std::size_t drawed = 0; std::size_t total = 0;

				for (auto &e : _cullableObjects)
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