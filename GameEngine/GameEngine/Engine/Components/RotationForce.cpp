#include <glm/gtc/matrix_transform.hpp>
#include "RotationForce.hh"
#include "Core/Engine.hh"

namespace Component
{

	RotationForce::RotationForce(Engine &engine, Handle &entity, const glm::vec3 &force)
		: Component::ComponentBase<RotationForce>(engine, entity, "RotationForce")
		, _force(force)
	{}

	RotationForce::~RotationForce(void)
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