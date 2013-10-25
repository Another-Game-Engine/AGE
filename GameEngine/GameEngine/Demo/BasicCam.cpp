#include "BasicCam.hh"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Core/Engine.hh"

BasicCam::BasicCam(void)
{
	setProjection() = glm::perspective(55.0f, 16.0f / 9.0f, 0.1f, 200.0f);
	setTransform() = glm::lookAt(glm::vec3(0, 5, -5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

BasicCam::~BasicCam(void)
{
}

void      BasicCam::customUpdate()
{
	float elapsed = GameEngine::instance()->timer().getElapsed();

	setTransform() = glm::rotate(getTransform(), 4.0f * elapsed, glm::vec3(1,0,0));
	setTransform() = glm::rotate(getTransform(), 10.0f * elapsed, glm::vec3(0,1,0));
	setTransform() = glm::rotate(getTransform(), 4.0f * elapsed, glm::vec3(0,0,1));
}
