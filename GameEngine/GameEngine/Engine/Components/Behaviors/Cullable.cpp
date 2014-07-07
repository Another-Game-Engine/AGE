#include "Cullable.hpp"
#include <Core/AScene.hh>

namespace AGE
{
	namespace ComponentBehavior
	{
		void Cullable::init(::AScene *scene, ENTITY_ID entityId, COMPONENT_ID componentTypeId)
		{
			_cullableId = scene->getInstance<AGE::Octree>()->addElement(componentTypeId);
			scene->getLink(entityId)->registerCullableId(_cullableId);
		}

		void Cullable::reset(::AScene *scene, ENTITY_ID entityId)
		{
			scene->getLink(entityId)->unregisterCullableId(_cullableId);
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