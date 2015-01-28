#include "RenderScene.hpp"
#include <Threads/PrepareRenderThread.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/Commands/Render.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

namespace AGE
{
	RenderScene::RenderScene(PrepareRenderThread *prepareThread, Engine *engine, AScene *scene)
		: _prepareThread(prepareThread)
		, _engine(engine)
		, _scene(scene)
		, _drawables(65536 * 4)
	{
		_drawablesToMove.reserve(65536);
		assert(prepareThread && engine && scene);
	}

	RenderScene::~RenderScene(void)
	{
	}

	bool RenderScene::init()
	{
		return true;
	}

	PrepareKey RenderScene::addMesh()
	{
		PrepareKey res;
		res.type = PrepareKey::Type::Drawable;
		res.id = _meshs.prepareAlloc();
		_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::CreateMesh>(res);
		return res;
	}

	PrepareKey RenderScene::addCamera()
	{
		PrepareKey res;
		res.type = PrepareKey::Type::Camera;
		res.id = _cameras.prepareAlloc();
		_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::CreateCamera>(res);
		return res;
	}

	PrepareKey RenderScene::addPointLight()
	{
		PrepareKey res;
		res.type = PrepareKey::Type::PointLight;
		res.id = _pointLights.prepareAlloc();
		_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::CreatePointLight>(res);
		return res;
	}

	RenderScene &RenderScene::removeElement(const PrepareKey &key)
	{
		assert(!key.invalid());
		switch (key.type)
		{
		case PrepareKey::Type::Camera:
			_cameras.prepareDealloc(key.id);
			_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::DeleteCamera>(key);
			break;
		case PrepareKey::Type::Drawable:
			_meshs.prepareDealloc(key.id);
			_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::DeleteDrawable>(key);
			break;
		case PrepareKey::Type::PointLight:
			_pointLights.prepareDealloc(key.id);
			_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::DeletePointLight>(key);
			break;
		default:
			break;
		}
		return (*this);
	}

	RenderScene &RenderScene::setPointLight(glm::vec3 const &color, glm::vec3 const &range, const PrepareKey &id)
	{
		_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::SetPointLight>(color, range, id);
		return (*this);
	}

	RenderScene &RenderScene::setPosition(const glm::vec3 &v, const PrepareKey &id)
	{
		_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::SetPosition>(id, v);
		return (*this);
	}

	RenderScene &RenderScene::setOrientation(const glm::quat &v, const PrepareKey &id)
	{
		_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::SetOrientation>(id, v);
		return (*this);
	}

	RenderScene &RenderScene::setScale(const glm::vec3 &v, const PrepareKey &id)
	{
		_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::SetScale>(id, v);
		return (*this);
	}

	RenderScene &RenderScene::setCameraInfos(const PrepareKey &id
		, const glm::mat4 &projection)
	{
		_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::CameraInfos>(id, projection);
		return (*this);
	}

	RenderScene &RenderScene::setPosition(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setPosition(v, e);
		return (*this);
	}

	RenderScene &RenderScene::setOrientation(const glm::quat &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setOrientation(v, e);
		return (*this);
	}

	RenderScene &RenderScene::setScale(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setScale(v, e);
		return (*this);
	}

	RenderScene &RenderScene::updateGeometry(
		const PrepareKey &key
		, const AGE::Vector<AGE::SubMeshInstance> &meshs
		, const AGE::Vector<AGE::MaterialInstance> &materials
		, const gl::Key<AGE::AnimationInstance> &animation)
	{
		assert(!key.invalid() || key.type != PrepareKey::Type::Drawable);
		_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::SetGeometry>(key, meshs, materials, animation);
		return (*this);
	}

	DRAWABLE_ID RenderScene::_addDrawable()
	{
		DRAWABLE_ID res = _drawables.alloc();
		_drawables.get(res).id = res;
		return res;
	}

	void RenderScene::removeDrawableObject(DRAWABLE_ID id)
	{
		Drawable &toRm = _drawables.get(id);
#ifdef ACTIVATE_OCTREE_CULLING
		if (toRm.hasMoved)
		{
			uint32_t idxMoveBuffer = toRm.moveBufferIdx;

			_drawablesToMove[idxMoveBuffer] = _drawablesToMove[_drawablesToMove.size() - 1];
			_drawables.get(_drawablesToMove[idxMoveBuffer]).moveBufferIdx = idxMoveBuffer;
			_drawablesToMove.pop_back();
		}
		// remove drawable from octree
		_octree.removeElement(&toRm);
#endif
		_drawables.dealloc(id);
		assert(id != (std::size_t)(-1));
	}

		void RenderScene::_setCameraInfos(AGE::Commands::MainToPrepare::CameraInfos &msg)
		{
			Camera *co = nullptr;
			co = &_cameras.get(msg.key.id);
			co->hasMoved = true;
			co->projection = msg.projection;
		}

		void RenderScene::_createCamera(AGE::Commands::MainToPrepare::CreateCamera &msg)
		{
			_cameras.allocPreparated(msg.key.id);
			Camera &toAdd = _cameras.get(msg.key.id);

			toAdd.activeCameraIdx = _activeCameras.size();
			_activeCameras.push_back(msg.key.id);

			toAdd.key.id = msg.key.id;
		}

		void RenderScene::_createPointLight(AGE::Commands::MainToPrepare::CreatePointLight &msg)
		{
			_pointLights.allocPreparated(msg.key.id);
			PointLight &toAdd = _pointLights.get(msg.key.id);

			// TODO: remove this
			toAdd.activePointLightIdx = _activePointLights.size();
			_activePointLights.push_back(msg.key.id);
			// ---
			toAdd.key.id = msg.key.id;
		}

		void RenderScene::_createMesh(AGE::Commands::MainToPrepare::CreateMesh &msg)
		{
			_meshs.allocPreparated(msg.key.id);
			Mesh &toAdd = _meshs.get(msg.key.id);
			toAdd.key.id = msg.key.id;
		}

		void RenderScene::_setPointLight(AGE::Commands::MainToPrepare::SetPointLight &msg)
		{
			PointLight *l = &_pointLights.get(msg.key.id);
			l->color = msg.color;
			l->range = msg.range;
		}
		
		void RenderScene::_deleteCamera(AGE::Commands::MainToPrepare::DeleteCamera &msg)
		{
			Camera &toRm = _cameras.get(msg.key.id);

			_activeCameras[toRm.activeCameraIdx] = _activeCameras[_activeCameras.size() - 1];
			_cameras.get(_activeCameras[toRm.activeCameraIdx]).activeCameraIdx = toRm.activeCameraIdx;
			_activeCameras.pop_back();

			_cameras.deallocPreparated(msg.key.id);
		}

		void RenderScene::_deletePointLight(AGE::Commands::MainToPrepare::DeletePointLight &msg)
		{
			PointLight &toRm = _pointLights.get(msg.key.id);

			_activePointLights[toRm.activePointLightIdx] = _activePointLights[_activePointLights.size() - 1];
			_pointLights.get(_activePointLights[toRm.activePointLightIdx]).activePointLightIdx = toRm.activePointLightIdx;
			_activePointLights.pop_back();

			_pointLights.deallocPreparated(msg.key.id);
		}

		void RenderScene::_deleteDrawable(AGE::Commands::MainToPrepare::DeleteDrawable &msg)
		{
			Mesh &toRm = _meshs.get(msg.key.id);
			for (auto &e : toRm.drawableCollection)
			{
				removeDrawableObject(e);
			}
			_meshs.deallocPreparated(msg.key.id);
		}

		void RenderScene::_setGeometry(AGE::Commands::MainToPrepare::SetGeometry &msg)
		{
			Mesh *uo = &_meshs.get(msg.key.id);

			for (auto &e : uo->drawableCollection)
				removeDrawableObject(e);
			uo->drawableCollection.clear();
			for (std::size_t i = 0; i < msg.submeshInstances.size(); ++i)
			{
				uint32_t id = _addDrawable();
				Drawable &added = _drawables.get(id);

				uo->drawableCollection.push_back(id);
				added.mesh = msg.submeshInstances[i];
				added.material = msg.materialInstances[i];
				if (!msg.materialInstances[i])
				{
					std::cout << "lolkillme";
					assert(false);
				}
				added.position = uo->position;
				added.orientation = uo->orientation;
				added.scale = uo->scale;
				added.meshAABB = msg.submeshInstances[i].boundingBox;
				added.animation = msg.animation;
				added.currentNode = UNDEFINED_IDX;
				added.transformation = glm::scale(glm::translate(glm::mat4(1),
															added.position) * glm::toMat4(added.orientation),
															added.scale);
				added.currentAABB.fromTransformedBox(added.meshAABB, added.transformation);
				added.previousAABB = added.currentAABB;
				added.hasMoved = false;
				_octree.addElement(&added);
				assert(added.currentNode != UNDEFINED_IDX);
			}
		}

		void RenderScene::_setPosition(AGE::Commands::MainToPrepare::SetPosition &msg)
		{
			Camera *co = nullptr;
			Mesh *uo = nullptr;
			PointLight *l = nullptr;

			switch (msg.key.type)
			{

			case(PrepareKey::Type::Camera) :
				co = &_cameras.get(msg.key.id);
				co->position = msg.position;
				co->hasMoved = true;
				break;
			case(PrepareKey::Type::Drawable) :
				uo = &_meshs.get(msg.key.id);
				uo->position = msg.position;
				for (uint32_t e : uo->drawableCollection)
				{
					_drawables.get(e).position = uo->position;
					assert(_drawables.get(e).currentNode != UNDEFINED_IDX);
					if (_drawables.get(e).hasMoved == false)
					{
						_drawables.get(e).hasMoved = true;
						_drawables.get(e).moveBufferIdx = _drawablesToMove.size();
						_drawablesToMove.push_back(e);
					}
				}
				break;
			case(PrepareKey::Type::PointLight) :
				l = &_pointLights.get(msg.key.id);
				l->position = msg.position;
				break;
			default:
				break;
			}
		}

		void RenderScene::_setScale(AGE::Commands::MainToPrepare::SetScale &msg)
		{
			Mesh *uo = nullptr;
			Camera *co = nullptr;
			switch (msg.key.type)
			{
			case(PrepareKey::Type::Camera) :
				co = &_cameras.get(msg.key.id);
				co->scale = msg.scale;
				co->hasMoved = true;
				break;
			case(PrepareKey::Type::Drawable) :
				uo = &_meshs.get(msg.key.id);
				uo->scale = msg.scale;
				for (auto &e : uo->drawableCollection)
				{
					_drawables.get(e).scale = uo->scale;
					assert(_drawables.get(e).currentNode != UNDEFINED_IDX);
					if (_drawables.get(e).hasMoved == false)
					{
						_drawables.get(e).hasMoved = true;
						_drawables.get(e).moveBufferIdx = _drawablesToMove.size();
						_drawablesToMove.push_back(e);
					}
				}
				break;
			default:
				break;
			}
		}

		void RenderScene::_setOrientation(AGE::Commands::MainToPrepare::SetOrientation &msg)
		{
			Mesh *uo = nullptr;
			Camera *co = nullptr;
			switch (msg.key.type)
			{
			case(PrepareKey::Type::Camera) :
				co = &_cameras.get(msg.key.id);
				co->orientation = msg.orientation;
				co->hasMoved = true;
				break;
			case(PrepareKey::Type::Drawable) :
				uo = &_meshs.get(msg.key.id);
				uo->orientation = msg.orientation;
				for (auto &e : uo->drawableCollection)
				{
					_drawables.get(e).orientation = uo->orientation;
					assert(_drawables.get(e).currentNode != UNDEFINED_IDX);
					if (_drawables.get(e).hasMoved == false)
					{
						_drawables.get(e).hasMoved = true;
						_drawables.get(e).moveBufferIdx = _drawablesToMove.size();
						_drawablesToMove.push_back(e);
					}
				}
				break;
			default:
				break;
			}
		}
		
		void RenderScene::_prepareDrawList(AGE::Commands::MainToPrepare::PrepareDrawLists &msg)
		{
			AGE::Vector<CullableObject*> toDraw;


			// we update animation instances
			//auto animationManager = getDependencyManager().lock()->getInstance<AGE::AnimationManager>();
			//animationManager->update(0.1f);


			// Update drawable positions in Octree
			for (uint32_t idx : _drawablesToMove)
			{
				Drawable *e = &_drawables.get(idx);
				assert(e->currentNode != UNDEFINED_IDX);
				e->hasMoved = false;
				e->previousAABB = e->currentAABB;
				e->transformation = glm::scale(glm::translate(glm::mat4(1), e->position) * glm::toMat4(e->orientation), e->scale);
				e->currentAABB.fromTransformedBox(e->meshAABB, e->transformation);
				_octree.moveElement(e);
				assert(e->currentNode != UNDEFINED_IDX);
			}
			_drawablesToMove.clear();
			// Do culling for each camera
			_octreeDrawList.clear();

			// clean empty nodes
			_octree.cleanOctree();

			for (uint32_t cameraIdx : _activeCameras)
			{
				Camera &camera = _cameras.get(cameraIdx);
				
				auto view = glm::inverse(glm::scale(glm::translate(glm::mat4(1), camera.position) * glm::toMat4(camera.orientation), camera.scale));

				// update frustum infos for culling
				camera.currentFrustum.setMatrix(camera.projection * view);

				_octreeDrawList.emplace_back();
				auto &drawList = _octreeDrawList.back();
				drawList.transformation = view;
				drawList.projection = camera.projection;

				// no culling for the lights for the moment (TODO)
				for (uint32_t pointLightIdx : _activePointLights)
				{
					auto &p = _pointLights.get(pointLightIdx);
					drawList.lights.emplace_back(p.position, p.color, p.range);
				}

#ifdef ACTIVATE_OCTREE_CULLING

				// Do the culling
				_octree.getElementsCollide(&camera, toDraw);

				// iter on element to draw
				for (CullableObject *e : toDraw)
				{
					// mandatory if you want the object to be found again
					e->hasBeenFound = false;
					// all the elements are drawable for the moment (TODO)
					Drawable *currentDrawable = dynamic_cast<Drawable*>(e);
					//if (!currentDrawable->animation.empty())
					//{
					//	drawList.drawables.emplace_back(currentDrawable->mesh, currentDrawable->material, currentDrawable->transformation, animationManager->getBones(currentDrawable->animation));
					//	drawList.drawables.back().animation = currentDrawable->animation;
					//}
					//else
					//{
						drawList.drawables.emplace_back(currentDrawable->mesh, currentDrawable->material, currentDrawable->transformation);
					//}
						if (!currentDrawable->material)
						{
							std::cout << "lol" << std::endl;
							assert(false);
						}
				}
#else
				for (auto &e : _drawables)
				{
					if (e.active)
					{
						drawList.drawables.emplace_back(e.mesh, e.material, e.transformation);
					}
				}
#endif
			}
			//getDependencyManager().lock()->getInstance<AGE::AnimationManager>()->update(0.1f);
				for (auto &i : this->_octreeDrawList)
				{
					for (auto &e : i.drawables)
					{
						if (!e.material)
						{
							std::cout << "lol";
							assert(false);
						}
					}
				}
			GetRenderThread()->getQueue()->emplaceCommand<Commands::Render::CopyDrawLists>(this->_octreeDrawList);
		}

}