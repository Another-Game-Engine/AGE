#pragma once

#include <Render/Key.hh>
#include <Render/Properties/IProperty.hh>
#include <AssetManagement/Instance/MaterialInstance.hh>
#include <AssetManagement/Instance/MeshInstance.hh>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <TMQ/message.hpp>

namespace AGE
{
	class Engine;
	struct DrawableCollection;
	class Painter;
	class Vertices;

	namespace Tasks
	{
		class Render
		{
		public:
			struct GetWindowSize : public TMQ::FutureData < glm::uvec2 >
			{
				GetWindowSize();
			};

			struct SetWindowSize
			{
				SetWindowSize(const glm::uvec2 & _size);
				glm::uvec2 size;
			};

			struct CreateRenderContext : TMQ::FutureData < bool >
			{
				CreateRenderContext(std::weak_ptr<Engine> _engine);
				std::shared_ptr<Engine> engine;
			};

			struct LoadMesh : TMQ::FutureData < bool >
			{

			};

			struct SetMeshMaterial
			{
				std::shared_ptr<MaterialSetInstance> material;
				std::shared_ptr<MeshInstance> mesh;

				SetMeshMaterial(std::shared_ptr<MaterialSetInstance> pMaterial,
					std::shared_ptr<MeshInstance> pMesh) :
					material(pMaterial),
					mesh(pMesh)
				{ }

			};

			struct RemoveMeshProperty
			{
				Key<Painter> meshPainter;
				Key<Properties> toRemove;
				int notNull;

				RemoveMeshProperty(Key<Painter> const &painter, Key<Properties> const &pToRemove);
			};

			struct SetMeshProperties
			{
				Key<Painter> meshPainter;
				Key<Properties> properties;
				std::shared_ptr<Properties> propertiesPtr;

				SetMeshProperties(Key<Painter> const &_meshPainter
					, Key<Properties> const &_properties
					, std::shared_ptr<Properties> const &_propertiesPtr) :
					meshPainter(_meshPainter)
					, properties(_properties)
					, propertiesPtr(_propertiesPtr)
				{ }
			};

			struct SetMeshTransform
			{
				Key<Painter> meshPainter;
				Key<Properties> meshProperties;
				Key<Property> transformProperty;
				glm::mat4 transformMat;

				SetMeshTransform(Key<Painter> pPainter,
					Key<Properties> pMeshProperties,
					Key<Property> pTransformProperty,
					glm::mat4 pTransformMat) :
					meshPainter(pPainter),
					meshProperties(pMeshProperties),
					transformProperty(pTransformProperty),
					transformMat(pTransformMat)
				{ }
			};
		};
	}
}