#include <glm/gtc/matrix_transform.hpp>
#include "RotationForce.hh"
#include "Core/Engine.hh"

namespace Component
{

	RotationForce::RotationForce(Engine &engine, const glm::vec3 &force)
		: Component::ComponentBase<RotationForce>(engine, "RotationForce")
		, _force(force)
	{}

	RotationForce::~RotationForce(void)
	{}

	//void RotationForce::update()
	//{
	//	float	elapsed = _engine.timer().getElapsed();

	//	getFather()->setLocalTransform() = glm::rotate(getFather()->getLocalTransform(), _force.x * elapsed, glm::vec3(1,0,0));
	//	getFather()->setLocalTransform() = glm::rotate(getFather()->getLocalTransform(), _force.y * elapsed, glm::vec3(0,1,0));
	//	getFather()->setLocalTransform() = glm::rotate(getFather()->getLocalTransform(), _force.z * elapsed, glm::vec3(0,0,1));
	//}


	void RotationForce::setForce(const glm::vec3 &force)
	{
		_force = force;
	}

	const glm::vec3 &RotationForce::getForce() const
	{
		return _force;
	}
};