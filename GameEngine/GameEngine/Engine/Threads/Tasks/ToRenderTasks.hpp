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
				std::weak_ptr<Engine> engine;
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

			struct CreateMeshProperty : public TMQ::FutureData<std::pair<Key<Properties>, Key<Property>>>
			{
				int toto;
			};

			struct RemoveMeshProperty
			{

			};

			struct SetMeshTransform
			{
				Key<Painter> painter;
				Key<Vertices> mesh;
				Key<Property> transform;
				glm::mat4 transformMat;

				SetMeshTransform(Key<Painter> pPainter,
					Key<Vertices> pMesh,
					Key<Property> pTransform,
					glm::mat4 pTransformMat);

			};

		};
	}
}