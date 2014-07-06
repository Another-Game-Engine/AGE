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
	}
}