#include "BasicCam.hh"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

BasicCam::BasicCam(void)
{
	setProjection() = glm::perspective(55.0f, 16.0f / 9.0f, 1.f, 1000.f);
	setTransform() = glm::lookAt(glm::vec3(57950, 0, -10), glm::vec3(57950, 0, 0), glm::vec3(0, 1, 0));
}

BasicCam::~BasicCam(void)
{
}

void      BasicCam::update()
{

}
