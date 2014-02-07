#include <Components/TrackBallComponent.hpp>

using namespace Component;

TrackBall::TrackBall() :
Component::ComponentBase<Component::TrackBall>(),
dist(0.0f),
rotateSpeed(0.0f),
zoomSpeed(0.0f),
angles(0, 0)
{}

TrackBall::~TrackBall(void)
{}

void TrackBall::init(Entity _toLook, float _dist, float rotatingSpeed, float zoomingSpeed)
{
	toLook = _toLook;
	dist = _dist;
	rotateSpeed = rotatingSpeed;
	zoomSpeed = zoomingSpeed;
}

void TrackBall::reset()
{}