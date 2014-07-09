#include "Camera.hpp"
#include <Core/AScene.hh>
#include <Core/Octree.hpp>

namespace AGE
{
	namespace ComponentBehavior
	{
		void Camera::init(::AScene *scene, ENTITY_ID entityId, COMPONENT_ID componentTypeId)
		{
			_cameraId = scene->getInstance<AGE::Octree>()->addElement(componentTypeId, scene->getEntityFromId(entityId));
			scene->getLink(entityId)->registerCullableId(_cameraId);
		}

		void Camera::reset(::AScene *scene, ENTITY_ID entityId)
		{
			scene->getLink(entityId)->unregisterCullableId(_cameraId);
		}
	}
}