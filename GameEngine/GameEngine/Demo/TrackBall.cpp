#include "TrackBall.hh"

#include "glm/gtc/matrix_transform.hpp"

TrackBall::TrackBall(SmartPointer<Entity> const &toLook, float dist,
					 float rotatingSpeed, float zoomingSpeed) :
	_dist(dist),
	_rotateSpeed(rotatingSpeed),
	_zoomSpeed(zoomingSpeed),
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

	_dist -= inputs.getMouseWheel().y * _zoomSpeed;
	if (inputs.getKey(SDL_BUTTON_LEFT))
		_angles -= glm::vec2(-(float)inputs.getMouseDelta().x / (1000 / _rotateSpeed), (float)inputs.getMouseDelta().y / (1000 / _rotateSpeed));
	if (abs(_dist) < 0.0001)
	{
		if (_dist < 0)
			_dist = -0.0001;
		else
			_dist = 0.0001;
	}
	pos.x = sin(_angles.x) * cos(_angles.y) * _dist;
	pos.y = sin(_angles.y) * _dist;
	pos.z = cos(_angles.x) * cos(_angles.y) * _dist;
	setTransform() = glm::lookAt(glm::vec3(_toLook->getGlobalTransform()[3]) + pos,
								 glm::vec3(_toLook->getGlobalTransform()[3]),
								 glm::vec3(0, 1, 0));
}
