#include "Camera.hpp"
#include <Core/AScene.hh>
#include <Core/PrepareRenderThread.hpp>

namespace AGE
{
	namespace ComponentBehavior
	{
		void Camera::init(::AScene *scene, ENTITY_ID entityId)
		{
			_scene = scene;
			_projection = glm::perspective(55.0f, 16.0f / 9.0f, 0.1f, 2000.0f);
			_cameraOTKey = scene->getInstance<AGE::Threads::Prepare>()->addCameraElement();
			scene->getLink(entityId)->registerOctreeObject(_cameraOTKey);
			scene->getInstance<AGE::Threads::Prepare>()->setCameraInfos(_cameraOTKey, _projection);
		}

		void Camera::reset(::AScene *scene, ENTITY_ID entityId)
		{
			scene->getLink(entityId)->unregisterOctreeObject(_cameraOTKey);
			scene->getInstance<AGE::Threads::Prepare>()->removeElement(_cameraOTKey);
		}

		void Camera::setProjection(const glm::mat4 &projection)
		{
			_projection = projection;
			_scene->getInstance<AGE::Threads::Prepare>()->setCameraInfos(_cameraOTKey, _projection);
		}

		const glm::mat4 &Camera::getProjection() const
		{
			return _projection;
		}

	}
}