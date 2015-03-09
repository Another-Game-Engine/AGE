#pragma once

#include <Utils/Containers/Queue.hpp>
#include <Utils/Containers/Vector.hpp>
#include <Utils/MemoryPool.hpp>
#include <Threads/Commands/MainToPrepareCommands.hpp>
#include <Threads/Tasks/MainToPrepareTasks.hpp>
#include <Core/PrepareKey.hpp>
#include <Entities/EntityTypedef.hpp>
#include <SpacePartitioning/LooseOctree.hh>
#include <SpacePartitioning/Cullable/Object/Mesh.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>
#include <array>

namespace AGE
{
	struct Drawable;
	struct Mesh;
	struct PointLight;
	struct Camera;
	struct DrawableCollection;
	class PrepareRenderThread;
	class Engine;
	class AScene;
	struct SubMeshInstance;
	struct Link;

	class RenderScene
	{
	public:
		RenderScene(PrepareRenderThread *prepareThread, Engine *engine, AScene *scene);
		~RenderScene(void);
		bool init();
		inline const AScene *getScene() const { return _scene; }
		inline Link *getRootLink() { return _rootLink.get(); }
	private:
		void _setCameraInfos(AGE::Commands::MainToPrepare::CameraInfos &msg);
		void _createCamera(AGE::Commands::MainToPrepare::CreateCamera &msg);
		void _createPointLight(AGE::Commands::MainToPrepare::CreatePointLight &msg);
		void _createMesh(AGE::Commands::MainToPrepare::CreateMesh &msg);
		void _setPointLight(AGE::Commands::MainToPrepare::SetPointLight &msg);
		void _deleteCamera(AGE::Commands::MainToPrepare::DeleteCamera &msg);
		void _deletePointLight(AGE::Commands::MainToPrepare::DeletePointLight &msg);
		void _deleteDrawable(AGE::Commands::MainToPrepare::DeleteMesh &msg);
		void _setGeometry(AGE::Commands::MainToPrepare::SetGeometry &msg);
		void _setTransform(AGE::Commands::MainToPrepare::SetTransform &msg);

		void _prepareDrawList(AGE::Commands::MainToPrepare::PrepareDrawLists &msg);
		void _moveElementsInOctree();

		Key<Properties> _createPropertiesContainer();

		void _removeProperties(const Key<Properties> &key);
		Key<Property> _attachProperty(const Key<Properties> &key, std::shared_ptr<IProperty> propertyPtr);
		void _detachProperty(const Key<Properties> &key, const Key<Property> &property);
		Key<Property> _addTransformationProperty(const Key<Properties> &propertiesKey, const glm::mat4 &value);

		template <typename PropertyType, typename ValueType>
		Key<Property> _createAndAttachProperty(const Key<Properties> &key, const ValueType &value)
		{
			std::shared_ptr<PropertyType> newProperty = std::make_shared<PropertyType>(value);
			return _attachProperty(key, newProperty);
		}

	public:
		PrepareKey addMesh();
		PrepareKey addCamera();
		PrepareKey addPointLight();
		RenderScene &removeElement(const PrepareKey &key);
		RenderScene &setTransform(const glm::mat4 &v, const PrepareKey &id);
		RenderScene &setTransform(const glm::mat4&v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);

		void removeDrawableObject(DRAWABLE_ID id);
	private:
		friend class PrepareRenderThread;
		AScene *_scene;
		PrepareRenderThread *_prepareThread;
		Engine *_engine;

		LooseOctree _octree;

		AGE::Vector<uint32_t> _drawablesToMove;
		AGE::Vector<uint32_t> _pointLightsToMove;

		MemoryPool<Mesh> _meshs;
		MemoryPool<Drawable> _drawables;
		MemoryPool<Camera> _cameras;
		MemoryPool<PointLight> _pointLights;

		AGE::Vector<uint32_t> _activeCameras;

		// TODO: remove active point lights and put it in the octree
		AGE::Vector<uint32_t> _activePointLights;

		std::array<RenderCameraListContainer, 3> _octreeDrawLists;

		std::unique_ptr<Link> _rootLink;

		MemoryPool<Properties> _properties;
	};
}