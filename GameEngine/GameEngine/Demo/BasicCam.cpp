#include "BasicCam.hh"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

BasicCam::BasicCam(void)
{
	setProjection() = glm::perspective(55.0f, 16.0f / 9.0f, 0.1f, 200.0f);
	setTransform() = glm::lookAt(glm::vec3(0, 10, -10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

BasicCam::~BasicCam(void)
{
}

void      BasicCam::update()
{

}
