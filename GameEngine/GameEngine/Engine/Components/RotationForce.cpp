#include <glm/gtc/matrix_transform.hpp>
#include "RotationForce.hh"
#include "Core/Engine.hh"

namespace Component
{

	RotationForce::RotationForce(Engine &engine, Handle &entity)
		: Component::ComponentBase<RotationForce>(engine, entity, "RotationForce")
		, _force(glm::vec3(0))
	{}

	RotationForce::~RotationForce(void)
	{}
	
	void RotationForce::init(const glm::vec3 &force)
	{
		_force = force;
	}

	void RotationForce::reset()
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