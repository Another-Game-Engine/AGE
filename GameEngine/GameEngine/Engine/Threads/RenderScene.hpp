#pragma once

#include <Utils/Containers/Queue.hpp>
#include <Utils/Containers/Vector.hpp>
#include <Utils/MemoryPool.hpp>
#include <Threads/Commands/MainToPrepareCommands.hpp>
#include <Core/PrepareKey.hpp>
#include <Entities/EntityTypedef.hpp>
#include <SpacePartitioning/LooseOctree.hh>
#include <SpacePartitioning/Cullable/Object/Mesh.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>
#include <array>

#include <Utils/MemoryPool.hpp>
#include <Render/Properties/IProperty.hh>
#include <Render/Properties/Transformation.hh> /* to remove*/ 

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
		void _setPosition(AGE::Commands::MainToPrepare::SetPosition &msg);
		void _setScale(AGE::Commands::MainToPrepare::SetScale &msg);
		void _setOrientation(AGE::Commands::MainToPrepare::SetOrientation &msg);
		void _prepareDrawList(AGE::Commands::MainToPrepare::PrepareDrawLists &msg);
		void _moveElementsInOctree();

		Key<Properties> _createPropertiesContainer()
		{
			return (Key<Properties>::createKey(_properties.alloc()));
		}

		void _removeProperties(const Key<Properties> &key)
		{
			_properties.dealloc(key.getId());
		}

		Key<Property> _attachProperty(const Key<Properties> &key, std::shared_ptr<IProperty> propertyPtr)
		{
			auto &properties = _properties.get(key.getId());

			return properties.add_property(propertyPtr);
		}

		template <typename PropertyType, typename ValueType>
		Key<Property> _createAndAttachProperty(const Key<Properties> &key, const ValueType &value)
		{
			std::shared_ptr<PropertyType> newProperty = std::make_shared<PropertyType>(value);

			return _attachProperty(key, newProperty);
		}

		Key<Property> _addTransformationProperty(const Key<Properties> &propertiesKey, const glm::mat4 &value)
		{
			return _createAndAttachProperty<Transformation>(propertiesKey, value);
		}

		MemoryPool<Properties> _properties;
		std::unordered_map<int, std::vector<Drawable*>> _verticeSorter;
	public:
		PrepareKey addMesh();
		PrepareKey addCamera();
		PrepareKey addPointLight();
		RenderScene &removeElement(const PrepareKey &key);
		RenderScene &setPointLight(glm::vec3 const &color, glm::vec3 const &range, const PrepareKey &id);
		RenderScene &setPosition(const glm::vec3 &v, const PrepareKey &id);
		RenderScene &setOrientation(const glm::quat &v, const PrepareKey &id);
		RenderScene &setScale(const glm::vec3 &v, const PrepareKey &id);
		RenderScene &setCameraInfos(const PrepareKey &id
			, const glm::mat4 &projection);
		RenderScene &setPosition(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
		RenderScene &setOrientation(const glm::quat &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
		RenderScene &setScale(const glm::vec3 &v, const std::array<PrepareKey, MAX_CPT_NUMBER> &ids);
		RenderScene &updateGeometry(
			const PrepareKey &key
			, const AGE::Vector<SubMeshInstance> &meshs);
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
	};
}