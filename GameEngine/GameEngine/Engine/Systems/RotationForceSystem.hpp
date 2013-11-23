#ifndef   __ROTATION_FORCE_SYSTEM_HPP__
# define  __ROTATION_FORCE_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/RotationForce.hh>
#include <Entities/Entity.hh>

class RotationForceSystem : public System
{
public:
	RotationForceSystem(){}
	virtual ~RotationForceSystem(){}
private:
	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		float t = time;
		for (auto &e : _collection)
		{
			glm::vec3 force = e->getComponent<Component::RotationForce>()->getForce();
			e->setLocalTransform() = glm::rotate(e->getLocalTransform(), force.x * t, glm::vec3(1, 0, 0));
			e->setLocalTransform() = glm::rotate(e->getLocalTransform(), force.y * t, glm::vec3(0, 1, 0));
			e->setLocalTransform() = glm::rotate(e->getLocalTransform(), force.z * t, glm::vec3(0, 0, 1));
		}
	}

	virtual void initialize()
	{
		require<Component::RotationForce>();
	}
};

#endif    //__ROTATION_FORCE_SYSTEM_HPP__