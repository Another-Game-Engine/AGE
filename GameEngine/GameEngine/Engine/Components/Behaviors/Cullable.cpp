#include "Cullable.hpp"
#include <Core/AScene.hh>
#include <Core/PrepareRenderThread.hpp>

#include <Render/Key.hh>
#include <Render/Data.hh>

namespace AGE
{
	namespace ComponentBehavior
	{
		void Cullable::init(::AScene *scene, ENTITY_ID entityId)
		{
			_scene = scene;
			assert(_cullableOTKey.invalid());
			_cullableOTKey = scene->getInstance<AGE::PrepareRenderThread>()->addCullableElement();
			scene->getLink(entityId)->registerOctreeObject(_cullableOTKey);
			assert(!_cullableOTKey.invalid());
		}

		void Cullable::reset(::AScene *scene, ENTITY_ID entityId)
		{
			assert(!_cullableOTKey.invalid());
			mesh = nullptr;
			material = nullptr;
			scene->getLink(entityId)->unregisterOctreeObject(_cullableOTKey);
			scene->getInstance<AGE::PrepareRenderThread>()->removeElement(_cullableOTKey);
			_cullableOTKey = PrepareKey();
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

			AGE::Vector<MaterialInstance> materials;
			for (auto &e : mesh->subMeshs)
			{
				if (e.defaultMaterialIndex >= material->datas.size())
					materials.push_back(material->datas[0]);
				else
					materials.push_back(material->datas[e.defaultMaterialIndex]);
			}
			_scene->getInstance<AGE::PrepareRenderThread>()->updateGeometry(_cullableOTKey, mesh->subMeshs, materials);
		}


	}
}