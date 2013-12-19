#include "BasicCam.hh"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Core/Engine.hh"
#include "Context/SdlContext.hh"

BasicCam::BasicCam(Engine &engine, Handle &toLook) : ACamera(engine),
	_dist(50),
	_lastKey(SDLK_r)
{
	_pos = glm::vec3(0, 5, -1);
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
	//glm::vec3	lookPos;
	//Input &input = _engine.getInstance<Input>();

	//if (_lastKey == SDLK_r)
	//	lookPos = glm::vec3(_toLook->getGlobalTransform()[3].x, _toLook->getGlobalTransform()[3].y, _toLook->getGlobalTransform()[3].z);
	//else if (_lastKey == SDLK_u)
	//{
	//	lookPos = glm::vec3(0, 0, 0);
	//}
	//else if (_lastKey == SDLK_SPACE)
	//{
	//	_pos = glm::vec3(_toLook->getGlobalTransform()[3].x + 1, _toLook->getGlobalTransform()[3].y, _toLook->getGlobalTransform()[3].z + _dist);
	//	lookPos = glm::vec3(_toLook->getGlobalTransform()[3].x, _toLook->getGlobalTransform()[3].y, _toLook->getGlobalTransform()[3].z);
	//}
	//if (input.getInput(SDLK_u))
	//{
	//	_pos = glm::vec3(2, 800, 0);
	//	_lastKey = SDLK_u;
	//}
	//if (input.getInput(SDLK_SPACE))
	//	_lastKey = SDLK_SPACE;
	//if (input.getInput(SDLK_r))
	//{
	//	_pos = glm::vec3(0, 5, -500);
	//	_lastKey = SDLK_r;
	//}
	//if (input.getInput(SDLK_UP))
	//	_dist -= 1;
	//if (input.getInput(SDLK_DOWN) || input.getInput(SDL_BUTTON_LEFT))
	//	_dist += 1;
	//setTransform() = glm::lookAt(_pos, lookPos, glm::vec3(0, 1, 0));
}
