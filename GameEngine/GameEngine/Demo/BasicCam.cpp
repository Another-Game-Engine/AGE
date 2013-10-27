#include "BasicCam.hh"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Core/Engine.hh"

BasicCam::BasicCam(SmartPointer<Entity> const &toLook) :
	_lastKey(SDLK_r)
{
	_pos = glm::vec3(0, 5, -500);
	_toLook = toLook;
	glm::vec3	lookPos = glm::vec3(_toLook->getGlobalTransform()[3].x, _toLook->getGlobalTransform()[3].y, _toLook->getGlobalTransform()[3].z);
	setProjection() = glm::perspective(55.0f, 16.0f / 9.0f, 0.1f, 2000.0f);
	setTransform() = glm::lookAt(_pos, lookPos, glm::vec3(0, 1, 0));
}

BasicCam::~BasicCam(void)
{
}

void      BasicCam::customUpdate()
{
	glm::vec3	lookPos;

	if (_lastKey == SDLK_r)
		lookPos = glm::vec3(_toLook->getGlobalTransform()[3].x, _toLook->getGlobalTransform()[3].y, _toLook->getGlobalTransform()[3].z);
	else if (_lastKey == SDLK_u)
	{
		_pos = glm::vec3(2, 800, 0);
		lookPos = glm::vec3(0, 0, 0);
	}
	else if (_lastKey == SDLK_SPACE)
	{
		_pos = glm::vec3(_toLook->getGlobalTransform()[3].x, _toLook->getGlobalTransform()[3].y, _toLook->getGlobalTransform()[3].z + 50);
		lookPos = glm::vec3(_toLook->getGlobalTransform()[3].x, _toLook->getGlobalTransform()[3].y, _toLook->getGlobalTransform()[3].z);
	}
	if (GameEngine::instance()->inputs().getInput(SDLK_u))
		_lastKey = SDLK_u;
	if (GameEngine::instance()->inputs().getInput(SDLK_SPACE))
		_lastKey = SDLK_SPACE;
	if (GameEngine::instance()->inputs().getInput(SDLK_r))
	{
		_pos = glm::vec3(0, 5, -500);
		_lastKey = SDLK_r;
	}
	setTransform() = glm::lookAt(_pos, lookPos, glm::vec3(0, 1, 0));
}
