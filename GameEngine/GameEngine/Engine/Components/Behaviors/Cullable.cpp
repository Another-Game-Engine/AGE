#include "Cullable.hpp"
#include <Core/AScene.hh>
#include <Core/Octree.hpp>

#include <OpenGL/Key.hh>
#include <OpenGL/Data.hh>

namespace AGE
{
	namespace ComponentBehavior
	{
		void Cullable::init(::AScene *scene, ENTITY_ID entityId)
		{
			_scene = scene;
			assert(_cullableId == (std::size_t)(-1));
			_cullableId = scene->getInstance<AGE::Octree>()->addElement();
			scene->getLink(entityId)->registerCullableId(_cullableId);
			assert(_cullableId != (std::size_t)(-1));
		}

		void Cullable::reset(::AScene *scene, ENTITY_ID entityId)
		{
			assert(_cullableId != (std::size_t)(-1));
			mesh = nullptr;
			material = nullptr;
			scene->getLink(entityId)->unregisterCullableId(_cullableId);
			scene->getInstance<AGE::Octree>()->removeElement(_cullableId);
			_cullableId = (std::size_t)(-1);
		}

		//temporary
		//
		//

		void Cullable::setMesh(const std::shared_ptr<AGE::MeshInstance> &_mesh)
		{
			mesh = _mesh;
			sendMeshInfos();
		}

		std::shared_ptr<AGE::MeshInstance> Cullable::getMesh()
		{
			return mesh;
		}

		void Cullable::setMaterial(const std::shared_ptr<AGE::MaterialSetInstance> &_material)
		{
			material = _material;
			sendMeshInfos();
		}

		std::shared_ptr<AGE::MaterialSetInstance> Cullable::getMaterial()
		{
			return material;
		}


		void Cullable::sendMeshInfos()
		{
			assert(_scene != nullptr);

			if (this->mesh == nullptr || this->material == nullptr)
			{
				return;
			}

			assert(material->datas.size() > 0);

			std::vector<MaterialInstance> materials;
			for (auto &e : mesh->subMeshs)
			{
				if (e.defaultMaterialIndex >= material->datas.size())
					materials.push_back(material->datas[0]);
				else
					materials.push_back(material->datas[e.defaultMaterialIndex]);
			}
			_scene->getInstance<AGE::Octree>()->updateGeometry(_cullableId, mesh->subMeshs, materials);
		}


	}
}