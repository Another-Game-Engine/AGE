#pragma once

#include <Entities/EntityTypedef.hpp>
#include <cstddef>
#include <Geometry/Mesh.hpp>
#include <Geometry/Material.hpp>

class AScene;

namespace AGE
{
	namespace ComponentBehavior
	{
		struct Cullable
		{
		private:
			std::size_t _cullableId;
		public:
			void init(::AScene *scene, ENTITY_ID entityId, COMPONENT_ID componentTypeId);
			void reset(::AScene *scene, ENTITY_ID entityId);

			Cullable()
				:_cullableId(std::size_t(-1))
				, mesh(nullptr)
				, material(nullptr)
			{}

			virtual ~Cullable()
			{}

			Cullable(Cullable &&o)
			{
				mesh = std::move(o.mesh);
				material = std::move(o.material);
				_cullableId = o._cullableId;
				_scene = o._scene;
			}

			Cullable &operator=(Cullable &&o)
			{
				mesh = std::move(o.mesh);
				material = std::move(o.material);
				_cullableId = o._cullableId;
				_scene = o._scene;
				return *this;
			}


			void setMesh(const std::shared_ptr<AGE::MeshInstance> &_mesh);
			std::shared_ptr<AGE::MeshInstance> getMesh();
			void setMaterial(const std::shared_ptr<AGE::MaterialSetInstance> &_mesh);
			std::shared_ptr<AGE::MaterialSetInstance> getMaterial();
		protected:
			std::shared_ptr<AGE::MeshInstance> mesh;
			std::shared_ptr<AGE::MaterialSetInstance> material;

			void sendMeshInfos();
		private:
			AScene *_scene = nullptr;
			Cullable(const Cullable &o);
			Cullable &operator=(const Cullable &o);
		};
	}
}