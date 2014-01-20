#include <glm/gtc/matrix_transform.hpp>
#include "RotationForce.hh"
#include "Core/Engine.hh"

namespace Component
{

	RotationForce::RotationForce()
		: Component::ComponentBase<RotationForce>()
		, force(glm::vec3(0))
	{}

	RotationForce::~RotationForce(void)
	{}
	
	void RotationForce::init(const glm::vec3 &_force)
	{
		force = _force;
	}

	void RotationForce::reset()
	{}


	void RotationForce::setForce(const glm::vec3 &_force)
	{
		force = _force;
	}

	const glm::vec3 &RotationForce::getForce() const
	{
		return force;
	}
};