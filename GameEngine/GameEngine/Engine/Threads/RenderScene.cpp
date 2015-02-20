#include <Threads/RenderScene.hpp>
#include <Threads/Commands/ToRenderCommands.hpp>
#include <Threads/Tasks/ToRenderTasks.hpp>
#include <Threads/PrepareRenderThread.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <SpacePartitioning/Cullable/Object/Camera.hh>
#include <SpacePartitioning/Cullable/Object/PointLight.hh>
#include <SpacePartitioning/Cullable/Object/Mesh.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>
#include <SpacePartitioning/Ouptut/RenderLight.hh>
#include <SpacePartitioning/Ouptut/RenderPainter.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>


namespace AGE
{
	RenderScene::RenderScene(PrepareRenderThread *prepareThread, Engine *engine, AScene *scene)
		: _prepareThread(prepareThread)
		, _engine(engine)
		, _scene(scene)
		, _octree(_drawables, _pointLights)
	{
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
		res.type = PrepareKey::Type::Mesh;
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
		case PrepareKey::Type::Mesh:
			_meshs.prepareDealloc(key.id);
			_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::DeleteMesh>(key);
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
		, const AGE::Vector<SubMeshInstance> &meshs)
	{
		assert(!key.invalid() || key.type != PrepareKey::Type::Mesh);
		_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::SetGeometry>(key, meshs);
		return (*this);
	}

	void RenderScene::removeDrawableObject(DRAWABLE_ID id)
	{
		Drawable &toRm = _drawables.get(id);
		AGE::GetRenderThread()->getQueue()->emplaceTask<AGE::Tasks::Render::RemoveMeshProperty>(toRm.mesh.painter, toRm.instanceProperties);
		if (toRm.hasMoved)
		{
			uint32_t idxMoveBuffer = toRm.moveBufferIdx;

			_drawablesToMove[idxMoveBuffer] = _drawablesToMove[_drawablesToMove.size() - 1];
			_drawables.get(_drawablesToMove[idxMoveBuffer]).moveBufferIdx = idxMoveBuffer;
			_drawablesToMove.pop_back();
		}
		// remove drawable from octree
		_octree.removeElement(&toRm);
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
			l->attenuation = msg.attenuation;
			l->hasMoved = true;
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

			// TODO: remove when point lights will be in octree
			_activePointLights[toRm.activePointLightIdx] = _activePointLights[_activePointLights.size() - 1];
			_pointLights.get(_activePointLights[toRm.activePointLightIdx]).activePointLightIdx = toRm.activePointLightIdx;
			_activePointLights.pop_back();
			// ---
			if (toRm.hasMoved)
			{
				_pointLightsToMove[toRm.moveBufferIdx] = _pointLightsToMove[_pointLightsToMove.size() - 1];
				_pointLights.get(_pointLightsToMove[toRm.moveBufferIdx]).moveBufferIdx = toRm.moveBufferIdx;
				_pointLightsToMove.pop_back();
			}

			_pointLights.deallocPreparated(msg.key.id);
		}

		void RenderScene::_deleteDrawable(AGE::Commands::MainToPrepare::DeleteMesh &msg)
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
				uint32_t id = _drawables.alloc();
				Drawable &added = _drawables.get(id);

				uo->drawableCollection.push_back(id);
				
				added.key.type = PrepareKey::Type::Drawable;
				added.key.id = id;

				added.mesh = msg.submeshInstances[i];

				auto addedProperty = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<AGE::Tasks::Render::CreateMeshProperty, std::pair<Key<Properties>, Key<Property>>>(added.mesh.painter);

				added.position = uo->position;
				added.orientation = uo->orientation;
				added.scale = uo->scale;
//				added.animation = msg.animation;
				added.currentNode = UNDEFINED_IDX;
				_drawablesToMove.push_back(id);
				added.hasMoved = true;
				auto propertyInfos = addedProperty.get();
				added.instanceProperties = propertyInfos.first;
				added.transformationProperty = propertyInfos.second;
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
			case(PrepareKey::Type::Mesh) :
				uo = &_meshs.get(msg.key.id);
				uo->position = msg.position;
				for (uint32_t e : uo->drawableCollection)
				{
					_drawables.get(e).position = uo->position;
					//assert(_drawables.get(e).currentNode != UNDEFINED_IDX);
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
				if (l->hasMoved == false)
				{
					l->hasMoved = true;
					l->moveBufferIdx = _pointLightsToMove.size();
					_pointLightsToMove.push_back(msg.key.id);
				}
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
			case(PrepareKey::Type::Mesh) :
				uo = &_meshs.get(msg.key.id);
				uo->scale = msg.scale;
				for (auto &e : uo->drawableCollection)
				{
					_drawables.get(e).scale = uo->scale;
					//assert(_drawables.get(e).currentNode != UNDEFINED_IDX);
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
			case(PrepareKey::Type::Mesh) :
				uo = &_meshs.get(msg.key.id);
				uo->orientation = msg.orientation;
				for (auto &e : uo->drawableCollection)
				{
					_drawables.get(e).orientation = uo->orientation;
					//assert(_drawables.get(e).currentNode != UNDEFINED_IDX);
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

		void RenderScene::_moveElementsInOctree()
		{
			for (uint32_t idx : _drawablesToMove)
			{
				Drawable &e = _drawables.get(idx);
				e.hasMoved = false;
				e.transformation = glm::scale(glm::translate(glm::mat4(1), e.position) * glm::toMat4(e.orientation), e.scale);
				e.shape.fromTransformedBox(e.mesh.boundingBox, e.transformation);
				if (e.currentNode == UNDEFINED_IDX)
					_octree.addElement(&e);
				else
					_octree.moveElement(&e);
				AGE::GetRenderThread()->getQueue()->emplaceTask<AGE::Tasks::Render::SetMeshTransform>(e.mesh.painter, e.instanceProperties, e.transformationProperty, e.transformation);
				assert(e.currentNode != UNDEFINED_IDX);
			}
			for (uint32_t idx : _pointLightsToMove)
			{
				PointLight &e = _pointLights.get(idx);

				e.hasMoved = false;
				e.computeSphereTransform();
				// TODO: move in octree
			}
			_drawablesToMove.clear();
			_pointLightsToMove.clear();
		}
		
		void RenderScene::_prepareDrawList(AGE::Commands::MainToPrepare::PrepareDrawLists &msg)
		{
			AGE::Vector<Cullable*> toDraw;

			_moveElementsInOctree();
			// Do culling for each camera
			_octreeDrawList.clear();
			// clean empty nodes
			_octree.cleanOctree();
			for (uint32_t cameraIdx : _activeCameras)
			{
				Camera &camera = _cameras.get(cameraIdx);
				auto view = glm::inverse(glm::scale(glm::translate(glm::mat4(1), camera.position) * glm::toMat4(camera.orientation), camera.scale));
				// update frustum infos for culling
				camera.shape.setMatrix(camera.projection * view);
				_octreeDrawList.emplace_back();
				auto &renderCamera = _octreeDrawList.back();
				renderCamera.camInfos.view = view;
				renderCamera.camInfos.projection = camera.projection;
				// no culling for the lights for the moment (TODO)
				for (uint32_t pointLightIdx : _activePointLights)
				{
					auto &p = _pointLights.get(pointLightIdx);
					renderCamera.lights.pointLight.emplace_back();
					renderCamera.lights.pointLight.back().light = p;
					// TODO: Cull the shadows
				}
				// Do the culling
				_octree.getElementsCollide(&camera, toDraw);
				// TODO: Remove that
				renderCamera.pipelines.emplace_back();
				// iter on elements to draw
				for (Cullable *e : toDraw)
				{
					switch (e->key.type)
					{
					case PrepareKey::Type::Drawable:
						{
							Drawable *currentDrawable = static_cast<Drawable*>(e);
							// TODO: get the pipeline idx of the mesh to render, here we use 0
							RenderPipeline *curRenderPipeline = &renderCamera.pipelines[0];
							RenderPainter *curRenderPainter = nullptr;

							for (auto &renderPainter : curRenderPipeline->keys)
							{
								if (renderPainter.painter == currentDrawable->mesh.painter)
								{
									curRenderPainter = &renderPainter;
									break;
								}
							}
							if (curRenderPainter == NULL)
							{
								curRenderPipeline->keys.emplace_back();
								curRenderPainter = &curRenderPipeline->keys.back();
								curRenderPainter->painter = currentDrawable->mesh.painter;
							}
							curRenderPainter->vertices.emplace_back(currentDrawable->mesh.vertices);
							curRenderPainter->properties.emplace_back(currentDrawable->instanceProperties);
						}
						break;
					case PrepareKey::Type::PointLight:
						{
							PointLight *currentPointLight = static_cast<PointLight*>(e);
							renderCamera.lights.pointLight.emplace_back();
							renderCamera.lights.pointLight.back().light = *currentPointLight;
							// TODO: Cull the shadows
						}
						break;
					default:
						assert(!"Type cannot be added to the ");
						break;
					}
					
				}
			}
			GetRenderThread()->getQueue()->emplaceCommand<Commands::ToRender::CopyDrawLists>(this->_octreeDrawList);
		}

}