#include "Camera.hpp"
#include <Core/AScene.hh>
#include <Core/Octree.hpp>

namespace AGE
{
	namespace ComponentBehavior
	{
		void Camera::init(::AScene *scene, ENTITY_ID entityId)
		{
			_scene = scene;
			_projection = glm::perspective(55.0f, 16.0f / 9.0f, 0.1f, 2000.0f);
			_cameraId = scene->getInstance<AGE::Octree>()->addCamera();
			scene->getInstance<AGE::Octree>()->setCameraInfos(_cameraId, _projection);
		}

		void Camera::reset(::AScene *scene, ENTITY_ID entityId)
		{
			scene->getInstance<AGE::Octree>()->removeCamera(_cameraId);
		}

		void Camera::setProjection(const glm::mat4 &projection)
		{
			_projection = projection;
			_scene->getInstance<AGE::Octree>()->setCameraInfos(_cameraId, _projection);
		}

		const glm::mat4 &Camera::getProjection() const
		{
			return _projection;
		}

	}
}