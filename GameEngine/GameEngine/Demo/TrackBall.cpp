#include "TrackBall.hh"

#include "glm/gtc/matrix_transform.hpp"

TrackBall::TrackBall(SmartPointer<Entity> const &toLook, float dist) :
	_dist(dist),
	_angles(0, 0),
	_toLook(toLook)
{
	setProjection() = glm::perspective(55.0f, 16.0f / 9.0f, 0.1f, 2000.0f);
}

TrackBall::~TrackBall(void)
{
}

void      TrackBall::customUpdate()
{
	Input			&inputs = GameEngine::instance()->inputs();
	glm::vec3		pos;

	if (inputs.getKey(SDLK_DOWN))
		_dist += GameEngine::instance()->timer().getElapsed() * 15;
	if (inputs.getKey(SDLK_UP))
		_dist -= GameEngine::instance()->timer().getElapsed() * 15;
	if (inputs.getKey(SDL_BUTTON_LEFT))
		_angles -= glm::vec2(-(float)inputs.getMouseDelta().x / 200, (float)inputs.getMouseDelta().y / 200);
	pos.x = sin(_angles.x) * cos(_angles.y) * _dist;
	pos.y = sin(_angles.y) * _dist;
	pos.z = cos(_angles.x) * cos(_angles.y) * _dist;
	setTransform() = glm::lookAt(glm::vec3(_toLook->getGlobalTransform()[3]) + pos,
								 glm::vec3(_toLook->getGlobalTransform()[3]),
								 glm::vec3(0, 1, 0));
}
