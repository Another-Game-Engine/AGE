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
#include <Culling/Cullable/Object/Camera.hh>
#include <Culling/Cullable/Object/PointLight.hh>
#include <Culling/Cullable/Object/Mesh.hh>
#include <Culling/Ouptut/RenderCamera.hh>
#include <Culling/Ouptut/RenderLight.hh>
#include <Culling/Ouptut/RenderPainter.hh>
#include <Culling/Ouptut/RenderPipeline.hh>
#include <Threads/RenderThread.hpp>
#include <Utils/Debug.hpp>
#include <Core/Link.hpp>
#include <Utils/MemoryPool.hpp>
#include <Render/Properties/IProperty.hh>
#include <Render/Properties/Transformation.hh>
#include <Utils/Profiler.hpp>

#include <Render/OcclusionTools/DepthMap.hpp>
#include <Render/OcclusionTools/DepthMapManager.hpp>
#include <Render/OcclusionTools/DepthMapHandle.hpp>

namespace AGE
{
	RenderScene::RenderScene(PrepareRenderThread *prepareThread, Engine *engine, AScene *scene)
		: _prepareThread(prepareThread)
		, _engine(engine)
		, _scene(scene)
		, _octree(_drawables, _pointLights)
	{
		assert(prepareThread && engine && scene);
		_rootLink = std::make_unique<Link>();
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
		res.id = (AGE::PrepareKey::OctreeObjectId)(_meshs.prepareAlloc());
		return res;
	}

	PrepareKey RenderScene::addCamera()
	{
		PrepareKey res;
		res.type = PrepareKey::Type::Camera;
		res.id = (AGE::PrepareKey::OctreeObjectId)(_cameras.prepareAlloc());
		_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::CreateCamera>(res);
		return res;
	}

	PrepareKey RenderScene::addPointLight()
	{
		PrepareKey res;
		res.type = PrepareKey::Type::PointLight;
		res.id = (AGE::PrepareKey::OctreeObjectId)(_pointLights.prepareAlloc());
		return res;
	}

	PrepareKey RenderScene::addDirectionalLight()
	{
		PrepareKey res;
		res.type = PrepareKey::Type::DirectionalLight;
		res.id = (AGE::PrepareKey::OctreeObjectId)(_directionalLights.prepareAlloc());
		return res;
	}

	PrepareKey RenderScene::addSpotLight()
	{
		PrepareKey res;
		res.type = PrepareKey::Type::SpotLight;
		res.id = (AGE::PrepareKey::OctreeObjectId)(_spotLights.prepareAlloc());
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
		case PrepareKey::Type::DirectionalLight:
			_directionalLights.prepareDealloc(key.id);
			_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::DeleteDirectionalLight>(key);
			break;
		case PrepareKey::Type::SpotLight:
			_spotLights.prepareDealloc(key.id);
			_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::DeleteSpotLight>(key);
			break;
		default:
			break;
		}
		return (*this);
	}

	RenderScene &RenderScene::setTransform(const glm::mat4 &v, const PrepareKey &id)
	{
		_prepareThread->getQueue()->emplaceCommand<Commands::MainToPrepare::SetTransform>(id, v);
		return (*this);
	}

	RenderScene &RenderScene::setTransform(const glm::mat4 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids)
	{
		for (auto &e : ids)
			setTransform(v, e);
		return (*this);
	}

	void RenderScene::removeDrawableObject(DRAWABLE_ID id)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Remove drawable object");

		Drawable &toRm = _drawables.get(id);
		_removeProperties(toRm.mesh.properties);
		if (toRm.hasMoved)
		{
			uint32_t idxMoveBuffer = toRm.moveBufferIdx;

			_drawablesToMove[idxMoveBuffer] = _drawablesToMove[_drawablesToMove.size() - 1];
			_drawables.get(_drawablesToMove[idxMoveBuffer]).moveBufferIdx = idxMoveBuffer;
			_drawablesToMove.pop_back();
		}
		// remove drawable from octree
		if (toRm.currentNode != UNDEFINED_IDX)
			_octree.removeElement(&toRm);
		_drawables.dealloc((uint32_t)(id));
		assert(id != (std::size_t)(-1));
	}

	void RenderScene::_setCameraInfos(AGE::Commands::MainToPrepare::CameraInfos &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Set camera infos");

		Camera *co = nullptr;
		co = &_cameras.get(msg.key.id);
		co->hasMoved = true;
		co->data = msg.data;
	}

	void RenderScene::_createCamera(AGE::Commands::MainToPrepare::CreateCamera &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Create camera");

		_cameras.allocPreparated(msg.key.id);
		Camera &toAdd = _cameras.get(msg.key.id);

		toAdd.activeCameraIdx = (uint32_t)(_activeCameras.size());
		_activeCameras.push_back(msg.key.id);

		toAdd.key.id = msg.key.id;
	}

	void RenderScene::_createPointLight(AGE::Commands::MainToPrepare::CreatePointLight &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Create point light");

		_pointLights.allocPreparated(msg.key.id);
		PointLight &toAdd = _pointLights.get(msg.key.id);

		// TODO: remove this
		toAdd.activePointLightIdx = (uint32_t)(_activePointLights.size());
		_activePointLights.push_back(msg.key.id);
		// ---
		toAdd.key.id = msg.key.id;
	}

	void RenderScene::_createSpotLight(AGE::Commands::MainToPrepare::CreateSpotLight &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Create spotlight");

		_spotLights.allocPreparated(msg.key.id);
		SpotLight &toAdd = _spotLights.get(msg.key.id);

		// TODO: remove this
		toAdd.activeSpotLightIdx = (uint32_t)(_activeSpotLights.size());
		_activeSpotLights.push_back(msg.key.id);
		// ---
		toAdd.key.id = msg.key.id;
	}

	void RenderScene::_createDirectionalLight(AGE::Commands::MainToPrepare::CreateDirectionalLight &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Create directional light");

		_directionalLights.allocPreparated(msg.key.id);
		DirectionalLight &toAdd = _directionalLights.get(msg.key.id);

		// TODO: remove this
		toAdd.activeDirectionalLightIdx = (uint32_t)(_activeDirectionalLights.size());
		_activeDirectionalLights.push_back(msg.key.id);
		// ---
		toAdd.key.id = msg.key.id;
	}

	void RenderScene::_createMesh(AGE::Commands::MainToPrepare::CreateMesh &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Create mesh");

		_meshs.allocPreparated(msg.key.id);
		Mesh &toAdd = _meshs.get(msg.key.id);
		toAdd.key.id = msg.key.id;
	}

	void RenderScene::_setPointLight(AGE::Commands::MainToPrepare::SetPointLight &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Set pointlight");

		PointLight *l = &_pointLights.get(msg.key.id);
		l->data = msg.data;
		if (l->hasMoved == false)
		{
			l->hasMoved = true;
			l->moveBufferIdx = static_cast<uint32_t>(_pointLightsToMove.size());
			_pointLightsToMove.push_back(msg.key.id);
		}
	}

	void RenderScene::_setSpotLight(AGE::Commands::MainToPrepare::SetSpotLight &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Set spotlight");

		SpotLight *l = &_spotLights.get(msg.key.id);
		l->data = msg.data;
		if (l->hasMoved == false)
		{
			l->hasMoved = true;
			l->moveBufferIdx = static_cast<uint32_t>(_spotLightsToMove.size());
			_spotLightsToMove.push_back(msg.key.id);
		}
	}

	void RenderScene::_setDirectionalLight(AGE::Commands::MainToPrepare::SetDirectionalLight &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Set directional light");

		DirectionalLight *l = &_directionalLights.get(msg.key.id);
		l->data = msg.data;
		if (l->hasMoved == false)
		{
			l->hasMoved = true;
		}
	}

	void RenderScene::_deleteCamera(AGE::Commands::MainToPrepare::DeleteCamera &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Delete camera");

		Camera &toRm = _cameras.get(msg.key.id);

		_activeCameras[toRm.activeCameraIdx] = _activeCameras[_activeCameras.size() - 1];
		_cameras.get(_activeCameras[toRm.activeCameraIdx]).activeCameraIdx = toRm.activeCameraIdx;
		_activeCameras.pop_back();

		_cameras.deallocPreparated(msg.key.id);
	}

	void RenderScene::_deletePointLight(AGE::Commands::MainToPrepare::DeletePointLight &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Delete point light");

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

	void RenderScene::_deleteDirectionalLight(AGE::Commands::MainToPrepare::DeleteDirectionalLight&msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Delete directionnal");

		DirectionalLight &toRm = _directionalLights.get(msg.key.id);
		_directionalLights.deallocPreparated(msg.key.id);
		// TODO: remove when point lights will be in octree
		_activeDirectionalLights[toRm.activeDirectionalLightIdx] = _activeDirectionalLights[_activeDirectionalLights.size() - 1];
		_directionalLights.get(_activeDirectionalLights[toRm.activeDirectionalLightIdx]).activeDirectionalLightIdx = toRm.activeDirectionalLightIdx;
		_activeDirectionalLights.pop_back();
		// ---
	}

	void RenderScene::_deleteSpotLight(AGE::Commands::MainToPrepare::DeleteSpotLight &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Delete spotlight");

		SpotLight &toRm = _spotLights.get(msg.key.id);

		// TODO: remove when point lights will be in octree
		_activeSpotLights[toRm.activeSpotLightIdx] = _activeSpotLights[_activeSpotLights.size() - 1];
		_spotLights.get(_activeSpotLights[toRm.activeSpotLightIdx]).activeSpotLightIdx = toRm.activeSpotLightIdx;
		_activeSpotLights.pop_back();
		// ---
		if (toRm.hasMoved)
		{
			_spotLightsToMove[toRm.moveBufferIdx] = _spotLightsToMove[_spotLightsToMove.size() - 1];
			_spotLights.get(_spotLightsToMove[toRm.moveBufferIdx]).moveBufferIdx = toRm.moveBufferIdx;
			_spotLightsToMove.pop_back();
		}

		_spotLights.deallocPreparated(msg.key.id);
	}

	void RenderScene::_deleteDrawable(AGE::Commands::MainToPrepare::DeleteMesh &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Delete drawable");

		Mesh &toRm = _meshs.get(msg.key.id);
		for (auto &e : toRm.drawableCollection)
		{
			removeDrawableObject(e);
		}
		_meshs.deallocPreparated(msg.key.id);
	}

	void RenderScene::_setGeometry(AGE::Commands::MainToPrepare::SetGeometry &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Set geometry");

		Mesh *uo = &_meshs.get(msg.key.id);

		for (auto &e : uo->drawableCollection)
		{
			removeDrawableObject(e);
		}
		uo->drawableCollection.clear();

		for (std::size_t i = 0; i < msg.submeshInstances.size(); ++i)
		{
			auto id = _drawables.alloc();
			Drawable &added = _drawables.get(id);
			auto &submesh = msg.submeshInstances[i];
			MaterialInstance *material = nullptr;
			if (submesh.defaultMaterialIndex < msg.submaterialInstances.size())
			{
				// correct material
				material = &(msg.submaterialInstances[submesh.defaultMaterialIndex]);
			}
			else
			{
				// material not found, first one
				material = &(msg.submaterialInstances[0]);
			}

			uo->drawableCollection.push_back(id);

			added.key.type = PrepareKey::Type::Drawable;
			added.key.id = (PrepareKey::OctreeObjectId)(id);

			added.mesh = msg.submeshInstances[i];

			added.transformation = uo->transformation;

			//				added.animation = msg.animation;
			added.currentNode = UNDEFINED_IDX;
			_drawablesToMove.push_back((PrepareKey::OctreeObjectId)(id));
			added.hasMoved = true;
			added.moveBufferIdx = (uint32_t)(_drawablesToMove.size() - 1);

			//if (added.mesh.properties.invalu)
			added.mesh.properties = _createPropertiesContainer();
			added.transformationProperty = _addTransformationProperty(added.mesh.properties, glm::mat4(1));

			// we remove old material properties
			for (auto &e : added.materialKeys)
			{
				_detachProperty(added.mesh.properties, e);
			}
			// we clear the old material key array
			added.materialKeys.clear();

			// we create new material properties and push keys in oldmaterial property array
			for (auto &e : material->_properties)
			{
				auto materialKey = _attachProperty(added.mesh.properties, e);
				added.materialKeys.push_back(materialKey);
			}
		}
	}

	void RenderScene::_setRenderMode(AGE::Commands::MainToPrepare::SetRenderMode &msg)
	{
		Mesh *uo = &_meshs.get(msg.key.id);

		AGE_ASSERT(uo != nullptr);

		for (auto &e : uo->drawableCollection)
		{
			Drawable &drawable = _drawables.get(e);

			drawable.renderMode = msg.renderModes;
		}
	}

	void RenderScene::_setTransform(AGE::Commands::MainToPrepare::SetTransform &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Set transform");

		Camera *co = nullptr;
		Mesh *uo = nullptr;
		PointLight *l = nullptr;
		SpotLight *s = nullptr;
		DirectionalLight *d = nullptr;
		switch (msg.key.type)
		{

		case(PrepareKey::Type::Camera) :
			if (!_cameras.exists(msg.key.id))
			{
				//if camera has been created then deleted in the same frame
				return;
			}
			co = &_cameras.get(msg.key.id);
			co->transformation = msg.transform;
			co->hasMoved = true;
			break;
		case(PrepareKey::Type::Mesh) :
			uo = &_meshs.get(msg.key.id);
			uo->transformation = msg.transform;
			for (auto e : uo->drawableCollection)
			{
				_drawables.get(e).transformation = uo->transformation;
				//assert(_drawables.get(e).currentNode != UNDEFINED_IDX);
				if (_drawables.get(e).hasMoved == false)
				{
					_drawables.get(e).hasMoved = true;
					_drawables.get(e).moveBufferIdx = (uint32_t)_drawablesToMove.size();
					_drawablesToMove.push_back((uint32_t)e);
				}
			}
			break;
		case(PrepareKey::Type::PointLight) :
			l = &_pointLights.get(msg.key.id);
			l->transformation = msg.transform;
			if (l->hasMoved == false)
			{
				l->hasMoved = true;
				l->moveBufferIdx = (uint32_t)_pointLightsToMove.size();
				_pointLightsToMove.push_back(msg.key.id);
			}
			break;
		case(PrepareKey::Type::SpotLight) :
			s = &_spotLights.get(msg.key.id);
			s->transformation = msg.transform;
			if (s->hasMoved == false)
			{
				s->hasMoved = true;
				s->moveBufferIdx = (uint32_t)_spotLightsToMove.size();
				_spotLightsToMove.push_back(msg.key.id);
			}
			break;
		case(PrepareKey::Type::DirectionalLight) :
			d = &_directionalLights.get(msg.key.id);
			d->transformation = msg.transform;
			break;
		default:
			break;
		}
	}

	void RenderScene::_moveElementsInOctree()
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Move element in octree");

		for (uint32_t idx : _drawablesToMove)
		{
			Drawable &e = _drawables.get(idx);
			e.hasMoved = false;
			e.shape.fromTransformedBox(e.mesh.boundingBox, e.transformation);
			if (e.currentNode == UNDEFINED_IDX)
				_octree.addElement(&e);
			else
				_octree.moveElement(&e);

			auto &properties = _properties.get(e.mesh.properties.getId());

			auto transformationProperty = properties.get_property<Transformation>(e.transformationProperty);
			transformationProperty->set(e.transformation);

			assert(e.currentNode != UNDEFINED_IDX);
		}
		for (uint32_t idx : _pointLightsToMove)
		{
			PointLight &e = _pointLights.get(idx);

			e.hasMoved = false;
			e.computeSphereTransform();
			// TODO: move in octree
		}
		for (uint32_t idx : _spotLightsToMove)
		{
			SpotLight &e = _spotLights.get(idx);
			e.hasMoved = false;
			// TODO: move in octree
		}
		_drawablesToMove.clear();
		_pointLightsToMove.clear();
		_spotLightsToMove.clear();
	}

	void RenderScene::_prepareDrawList(AGE::Commands::MainToPrepare::PrepareDrawLists &msg)
	{
		SCOPE_profile_cpu_i("PrepareTimer", "Prepare draw list");

		AGE::Vector<Cullable*> toDraw;

		_moveElementsInOctree();
		// Do culling for each camera

		RenderCameraListContainer *drawContainer = nullptr;

		// We search an unused draw list
		for (auto &e : _octreeDrawLists)
		{
			if (e.used == false)
			{
				SCOPE_profile_cpu_i("PrepareTimer", "Clear old draw list");
				drawContainer = &e;
				drawContainer->used = true;
				drawContainer->cameras.clear();
				break;
			}
		}

		// id all draw lists are used by render thread
		// we discard this draw
		if (drawContainer == nullptr)
			return;

		// clean empty nodes
		_octree.cleanOctree();

		for (uint32_t cameraIdx : _activeCameras)
		{
			auto scopeName = "Prepare camera " + std::to_string(cameraIdx);
			SCOPE_profile_cpu_i("PrepareTimer", scopeName.c_str());

			Camera &camera = _cameras.get(cameraIdx);
			//				auto view = glm::inverse(glm::scale(glm::translate(glm::mat4(1), camera.position) * glm::toMat4(camera.orientation), camera.scale));
			auto view = glm::inverse(camera.transformation);

			// update frustum infos for culling
			camera.shape.setMatrix(camera.data.projection * view);

			auto &drawList = drawContainer->cameras;

			drawList.emplace_back();
			auto &renderCamera = drawList.back();
			renderCamera.camInfos.view = view;
			renderCamera.camInfos.data = camera.data;

			auto VP = camera.data.projection * view;

			auto &depthMapManager = GetRenderThread()->getDepthMapManager();
			auto depthMap = depthMapManager.getReadableMap();

			// no culling for the lights for the moment (TODO)
			{
				SCOPE_profile_cpu_i("PrepareTimer", "Pointlights preparation");
				for (uint32_t pointLightIdx : _activePointLights)
				{
					auto &p = _pointLights.get(pointLightIdx);
					renderCamera.lights.pointLight.emplace_back();
					RenderLight<PointLight> *curLight = &renderCamera.lights.pointLight.back();
					curLight->light = p;
					// TODO: Cull the shadows
					// VERY UGLY CULLING FOR THE MOMENT
					AGE::Vector<Cullable*> objectsInShadow;
					_octree.getAllElements(objectsInShadow);

					for (Cullable *e : objectsInShadow)
					{
						switch (e->key.type)
						{
						case PrepareKey::Type::Drawable:
						{
							Drawable *currentDrawable = static_cast<Drawable*>(e);
							RenderPainter *curRenderPainter = nullptr;
							RenderDrawableList *curRenderDrawablelist = nullptr;

							auto renderPainter = curLight->keys.find(currentDrawable->mesh.painter.getId());
							// We find the good render painter
							if (renderPainter == curLight->keys.end())
							{
								curRenderPainter = &curLight->keys[currentDrawable->mesh.painter.getId()];
							}
							else
								curRenderPainter = &renderPainter->second;
							// and the good render mode
							for (auto &drawableList : curRenderPainter->drawables)
							{
								if (drawableList.renderMode == currentDrawable->renderMode)
								{
									curRenderDrawablelist = &drawableList;
									break;
								}
							}
							if (curRenderDrawablelist == nullptr)
							{
								curRenderPainter->drawables.emplace_back();
								curRenderDrawablelist = &curRenderPainter->drawables.back();
								curRenderDrawablelist->renderMode = currentDrawable->renderMode;
							}
							curRenderDrawablelist->vertices.emplace_back(currentDrawable->mesh.vertices);
							curRenderDrawablelist->properties.emplace_back(_properties.get(currentDrawable->mesh.properties.getId()));
						}
						}
					}
					// END OF VERY UGLY CULLING
				}
			}
			{
				SCOPE_profile_cpu_i("PrepareTimer", "Spotlights preparation");
				for (uint32_t spotLightIdx : _activeSpotLights)
				{
					auto &p = _spotLights.get(spotLightIdx);
					renderCamera.lights.spotLights.emplace_back();
					RenderLight<SpotLight> *curLight = &renderCamera.lights.spotLights.back();
					curLight->light = p;
					// TODO: Cull the shadows
					// VERY UGLY CULLING FOR THE MOMENT
					AGE::Vector<Cullable*> objectsInShadow;
					_octree.getAllElements(objectsInShadow);
					for (Cullable *e : objectsInShadow)
					{
						switch (e->key.type)
						{
						case PrepareKey::Type::Drawable:
						{
							Drawable *currentDrawable = static_cast<Drawable*>(e);
							RenderPainter *curRenderPainter = nullptr;
							RenderDrawableList *curRenderDrawablelist = nullptr;

							auto renderPainter = curLight->keys.find(currentDrawable->mesh.painter.getId());
							// We find the good render painter
							if (renderPainter == curLight->keys.end())
							{
								curRenderPainter = &curLight->keys[currentDrawable->mesh.painter.getId()];
							}
							else
								curRenderPainter = &renderPainter->second;
							// and the good render mode
							for (auto &drawableList : curRenderPainter->drawables)
							{
								if (drawableList.renderMode == currentDrawable->renderMode)
								{
									curRenderDrawablelist = &drawableList;
									break;
								}
							}
							if (curRenderDrawablelist == nullptr)
							{
								curRenderPainter->drawables.emplace_back();
								curRenderDrawablelist = &curRenderPainter->drawables.back();
								curRenderDrawablelist->renderMode = currentDrawable->renderMode;
							}
							curRenderDrawablelist->vertices.emplace_back(currentDrawable->mesh.vertices);
							curRenderDrawablelist->properties.emplace_back(_properties.get(currentDrawable->mesh.properties.getId()));
						}
						}
					}
					// END OF VERY UGLY CULLING
				}
			}

			// no culling possible on directional light so paul you don't have to do it ! is it nice ?
			// ANSWER: Well, it's actually gonna be even harder to cull this...
			// TODO: compute a box for the shadows (orthogonal frustum) and cull the objects :(
			for (uint32_t directionalLightIdx : _activeDirectionalLights)
			{
				auto &p = _directionalLights.get(directionalLightIdx);
				renderCamera.lights.directionalLights.emplace_back();
				renderCamera.lights.directionalLights.back().light = p;
			}

			{
				SCOPE_profile_cpu_i("PrepareTimer", "Drawable culling");
				// Do the culling
				_octree.getElementsCollide(&camera, toDraw);
				RenderPipeline *curRenderPipeline = &renderCamera.pipeline;
				// iter on elements to draw
				for (Cullable *e : toDraw)
				{
					switch (e->key.type)
					{
					case PrepareKey::Type::Drawable:
					{
						Drawable *currentDrawable = static_cast<Drawable*>(e);

						RenderPainter *curRenderPainter = nullptr;
						RenderDrawableList *curRenderDrawablelist = nullptr;

						bool drawObject = true;

						auto renderPainter = curRenderPipeline->keys.find(currentDrawable->mesh.painter.getId());
						// We find the good render painter
						if (renderPainter == curRenderPipeline->keys.end())
						{
							curRenderPainter = &curRenderPipeline->keys[currentDrawable->mesh.painter.getId()];
						}
						else
							curRenderPainter = &renderPainter->second;
						// and the good render mode
						for (auto &drawableList : curRenderPainter->drawables)
						{
							if (drawableList.renderMode == currentDrawable->renderMode)
							{
								curRenderDrawablelist = &drawableList;
								break;
							}
						}
						if (curRenderDrawablelist == nullptr)
						{
							curRenderPainter->drawables.emplace_back();
							curRenderDrawablelist = &curRenderPainter->drawables.back();
							curRenderDrawablelist->renderMode = currentDrawable->renderMode;
						}


#ifdef OCCLUSION_CULLING
						{
							SCOPE_profile_cpu_i("PrepareTimer", "Occlusion culling");

							if (depthMap.isValid() && curRenderDrawablelist->renderMode.at(AGE_OCCLUDER) == false)
							{
								drawObject = false;

								auto BB = currentDrawable->mesh.boundingBox;

								glm::vec2 minPoint = glm::vec2(1);
								glm::vec2 maxPoint = glm::vec2(-1);

								float minZ = std::numeric_limits<float>::max();

								for (std::size_t i = 0; i < 8; ++i)
								{
									auto point = depthMap->getMV() * currentDrawable->transformation * glm::vec4(BB.getCornerPoint(i), 1.0f);
									point /= point.w;

									if (point.x < -1)
									{
										point.x = -1;
									}
									if (point.y < -1)
									{
										point.y = -1;
									}
									if (point.x > 1)
									{
										point.x = 1;
									}
									if (point.y > 1)
									{
										point.y = 1;
									}

									minPoint.x = std::min(minPoint.x, point.x);
									minPoint.y = std::min(minPoint.y, point.y);
									maxPoint.x = std::max(maxPoint.x, point.x);
									maxPoint.y = std::max(maxPoint.y, point.y);

									point.z = (point.z + 1.0f) * 0.5f;
									minZ = std::min(minZ, point.z);
								}

								glm::uvec2 screenMin(((minPoint + glm::vec2(1)) / glm::vec2(2)) * glm::vec2(depthMap->getMipmapWidth(), depthMap->getMipmapHeight()));
								glm::uvec2 screenMax(((maxPoint + glm::vec2(1)) / glm::vec2(2)) * glm::vec2(depthMap->getMipmapWidth(), depthMap->getMipmapHeight()));

								if (minZ < 0)
								{
									minZ = 0;
								}

								drawObject |= depthMap->testBox((uint32_t)(minZ * (1 << 24)), screenMin, screenMax);

								if (drawObject)
								{
									GetRenderThread()->getQueue()->emplaceCommand<AGE::Commands::ToRender::Draw2DLine>(glm::vec2(minPoint.x, minPoint.y), glm::vec2(minPoint.x, maxPoint.y));
									GetRenderThread()->getQueue()->emplaceCommand<AGE::Commands::ToRender::Draw2DLine>(glm::vec2(minPoint.x, maxPoint.y), glm::vec2(maxPoint.x, maxPoint.y));
									GetRenderThread()->getQueue()->emplaceCommand<AGE::Commands::ToRender::Draw2DLine>(glm::vec2(maxPoint.x, maxPoint.y), glm::vec2(maxPoint.x, minPoint.y));
									GetRenderThread()->getQueue()->emplaceCommand<AGE::Commands::ToRender::Draw2DLine>(glm::vec2(maxPoint.x, minPoint.y), glm::vec2(minPoint.x, minPoint.y));
								}
							}
						}
#endif // OCCLUSION
						if (drawObject)
						{
							curRenderDrawablelist->vertices.emplace_back(currentDrawable->mesh.vertices);
							curRenderDrawablelist->properties.emplace_back(_properties.get(currentDrawable->mesh.properties.getId()));
						}

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
					case PrepareKey::Type::SpotLight:
					{
						SpotLight *currentSpotLight = static_cast<SpotLight*>(e);
						renderCamera.lights.spotLights.emplace_back();
						renderCamera.lights.spotLights.back().light = *currentSpotLight;
					}
					break;
					default:
						assert(!"Type cannot be added to the render queue.");
						break;
					}
				}
			}
		}
		GetRenderThread()->getQueue()->emplaceCommand<Commands::ToRender::CopyDrawLists>(std::make_shared<RenderCameraListContainerHandle>(*drawContainer));
	}

	Key<Properties> RenderScene::_createPropertiesContainer()
	{
		return (Key<Properties>::createKey(_properties.alloc()));
	}

	void RenderScene::_removeProperties(const Key<Properties> &key)
	{
		_properties.dealloc(key.getId());
	}

	Key<Property> RenderScene::_attachProperty(const Key<Properties> &key, std::shared_ptr<IProperty> propertyPtr)
	{
		auto &properties = _properties.get(key.getId());

		return properties.add_property(propertyPtr);
	}

	void RenderScene::_detachProperty(const Key<Properties> &key, const Key<Property> &prop)
	{
		auto &properties = _properties.get(key.getId());

		properties.remove_property(prop);
	}

	Key<Property> RenderScene::_addTransformationProperty(const Key<Properties> &propertiesKey, const glm::mat4 &value)
	{
		return _createAndAttachProperty<Transformation>(propertiesKey, value);
	}

}