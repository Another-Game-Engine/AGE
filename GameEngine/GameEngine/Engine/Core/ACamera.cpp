
#include "ACamera.hh"

ACamera::ACamera() :
	_hasMoved(true)
{
}

glm::mat4         &ACamera::setTransform()
{
	_hasMoved = true;
	return (_transform);
}

glm::mat4 		  &ACamera::getTransform()
{
	return (_transform);
}

glm::mat4         &ACamera::setProjection()
{
	_hasMoved = true;
	return (_projection);
}

glm::mat4 		  &ACamera::getProjection()
{
	return (_projection);
}

bool              ACamera::hasMoved() const
{
	return (_hasMoved);
}

void              ACamera::resetMoved()
{
	_hasMoved = false;
}
