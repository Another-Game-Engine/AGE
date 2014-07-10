#include "Cullable.hpp"
#include <Core/AScene.hh>
#include <Core/Octree.hpp>

namespace AGE
{
	namespace ComponentBehavior
	{
		void Cullable::init(::AScene *scene, ENTITY_ID entityId, COMPONENT_ID componentTypeId)
		{
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
			shader = "";
			scene->getLink(entityId)->unregisterCullableId(_cullableId);
			scene->getInstance<AGE::Octree>()->removeElement(_cullableId);
			_cullableId = (std::size_t)(-1);
			draw = false;
		}

		//temporary
		//
		//

		void Cullable::setShader(const std::string &_shader)
		{
			shader = _shader;
		}

		void Cullable::setMesh(const std::shared_ptr<ObjFile> &_mesh)
		{
			mesh = _mesh;
		}

		std::shared_ptr<ObjFile> Cullable::getMesh()
		{
			return mesh;
		}

		const std::string &Cullable::getShader()
		{
			return shader;
		}

	}
}