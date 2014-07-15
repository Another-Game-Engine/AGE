#include "Cullable.hpp"
#include <Core/AScene.hh>
#include <Core/Octree.hpp>

#include <OpenGL/Key.hh>
#include <OpenGL/Data.hh>

namespace AGE
{
	namespace ComponentBehavior
	{
		void Cullable::init(::AScene *scene, ENTITY_ID entityId, COMPONENT_ID componentTypeId)
		{
			_scene = scene;
			assert(_cullableId == (std::size_t)(-1));
			_cullableId = scene->getInstance<AGE::Octree>()->addElement(componentTypeId, scene->getEntityFromId(entityId));
			scene->getLink(entityId)->registerCullableId(_cullableId);
			draw = false;
			assert(_cullableId != (std::size_t)(-1));
		}

		void Cullable::reset(::AScene *scene, ENTITY_ID entityId)
		{
			assert(_cullableId != (std::size_t)(-1));
			mesh = nullptr;
			scene->getLink(entityId)->unregisterCullableId(_cullableId);
			scene->getInstance<AGE::Octree>()->removeElement(_cullableId);
			_cullableId = (std::size_t)(-1);
			draw = false;
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

		void Cullable::sendMeshInfos()
		{
			assert(_scene != nullptr);

			if (this->mesh == nullptr)
			{
				std::vector<SubMeshInstance> subMeshs;
				_scene->getInstance<AGE::Octree>()->updateGeometry(_cullableId, subMeshs);
				return;
			}
			_scene->getInstance<AGE::Octree>()->updateGeometry(_cullableId, mesh->subMeshs);
		}


	}
}