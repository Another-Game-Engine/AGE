#include <glm/gtc/matrix_transform.hpp>
#include "RotationForce.hh"
#include "Core/Engine.hh"

namespace Components
{

	RotationForce::RotationForce(const glm::vec3 &force)
		: AComponent("RotationForce")
		, _force(force)
	{}

	RotationForce::~RotationForce(void)
	{}

	void RotationForce::start()
	{}

	void RotationForce::update()
	{
		float	elapsed = GameEngine::instance()->timer().getElapsed();

		getFather()->setLocalTransform() = glm::rotate(getFather()->getLocalTransform(), _force.x * elapsed, glm::vec3(1,0,0));
		getFather()->setLocalTransform() = glm::rotate(getFather()->getLocalTransform(), _force.y * elapsed, glm::vec3(0,1,0));
		getFather()->setLocalTransform() = glm::rotate(getFather()->getLocalTransform(), _force.z * elapsed, glm::vec3(0,0,1));
	}

	void RotationForce::stop()
	{}

	void RotationForce::setForce(const glm::vec3 &force)
	{
		_force = force;
	}

	const glm::vec3 &RotationForce::getForce() const
	{
		return _force;
	}
};