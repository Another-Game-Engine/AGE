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
#include <Threads/RenderThread.hpp>
#include <Utils/Debug.hpp>
#include <Core/Link.hpp>
#include <Utils/MemoryPool.hpp>
#include <Render/Properties/IProperty.hh>
#include <Render/Properties/Transformation.hh>

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
		res.id = _meshs.prepareAlloc();
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
		co->pipelines.clear();
		for (auto &e : msg.pipelines)
		{
			co->pipelines.push_back(e);
		}
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
		{
			removeDrawableObject(e);
		}
		uo->drawableCollection.clear();
		
		for (std::size_t i = 0; i < msg.submeshInstances.size(); ++i)
		{
			uint32_t id = _drawables.alloc();
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
			added.key.id = id;

			added.mesh = msg.submeshInstances[i];

			added.transformation = uo->transformation;

			//				added.animation = msg.animation;
			added.currentNode = UNDEFINED_IDX;
			_drawablesToMove.push_back(id);
			added.hasMoved = true;
			added.moveBufferIdx = _drawablesToMove.size() - 1;

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

	void RenderScene::_setTransform(AGE::Commands::MainToPrepare::SetTransform &msg)
	{
		Camera *co = nullptr;
		Mesh *uo = nullptr;
		PointLight *l = nullptr;

		switch (msg.key.type)
		{

		case(PrepareKey::Type::Camera) :
			co = &_cameras.get(msg.key.id);
			co->transformation = msg.transform;
			co->hasMoved = true;
			break;
		case(PrepareKey::Type::Mesh) :
			uo = &_meshs.get(msg.key.id);
			uo->transformation = msg.transform;
			for (uint32_t e : uo->drawableCollection)
			{
				_drawables.get(e).transformation = uo->transformation;
				//assert(_drawables.get(e).currentNode != UNDEFINED_IDX);
				if (_drawables.get(e).hasMoved == false)
				{
					_drawables.get(e).hasMoved = true;
					_drawables.get(e).moveBufferIdx = (size_t)_drawablesToMove.size();
					_drawablesToMove.push_back(e);
				}
			}
			break;
		case(PrepareKey::Type::PointLight) :
			l = &_pointLights.get(msg.key.id);
			l->transformation = msg.transform;
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

	void RenderScene::_moveElementsInOctree()
	{
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
		_drawablesToMove.clear();
		_pointLightsToMove.clear();
	}

	void RenderScene::_prepareDrawList(AGE::Commands::MainToPrepare::PrepareDrawLists &msg)
	{
		AGE::Vector<Cullable*> toDraw;

		_moveElementsInOctree();
		// Do culling for each camera

		RenderCameraListContainer *drawContainer = nullptr;

		// We search an unused draw list
		for (auto &e : _octreeDrawLists)
		{
			if (e.used == false)
			{
				drawContainer = &e;
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
			Camera &camera = _cameras.get(cameraIdx);
			//				auto view = glm::inverse(glm::scale(glm::translate(glm::mat4(1), camera.position) * glm::toMat4(camera.orientation), camera.scale));
			auto view = glm::inverse(camera.transformation);

			// update frustum infos for culling
			camera.shape.setMatrix(camera.projection * view);

			auto &drawList = drawContainer->cameras;

			drawList.emplace_back();
			auto &renderCamera = drawList.back();
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

			// for each render pipeline of camera
			for (auto &pipelineId : camera.pipelines)
			{
				renderCamera.pipelines.resize(pipelineId + 1);
				RenderPipeline *curRenderPipeline = &renderCamera.pipelines[pipelineId];

				// iter on elements to draw
				for (Cullable *e : toDraw)
				{
					switch (e->key.type)
					{
					case PrepareKey::Type::Drawable:
					{
						Drawable *currentDrawable = static_cast<Drawable*>(e);

						RenderPainter *curRenderPainter = nullptr;

						for (auto &renderPainter : curRenderPipeline->keys)
						{
							if (renderPainter.painter.getId() == currentDrawable->mesh.painter.getId())
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
						curRenderPainter->properties.emplace_back(_properties.get(currentDrawable->mesh.properties.getId()));

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
		}
		drawContainer->used = true;
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