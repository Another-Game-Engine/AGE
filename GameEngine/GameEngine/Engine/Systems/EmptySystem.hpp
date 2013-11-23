#ifndef   __EMPTY_SYSTEM_HPP__
# define  __EMPTY_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/RotationForce.hh>
#include <Entities/Entity.hh>

class EmptySystem : public System
{
public:
	EmptySystem(){}
	virtual ~EmptySystem(){}
private:
	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		for (auto &e : _collection)
		{
			glm::vec3 force = e->getComponent<Component::RotationForce>()->getForce();
			float t = time;
			e->getFather()->setLocalTransform() = glm::rotate(e->getFather()->getLocalTransform(), force.x * t, glm::vec3(1, 0, 0));
			e->getFather()->setLocalTransform() = glm::rotate(e->getFather()->getLocalTransform(), force.y * t, glm::vec3(0, 1, 0));
			e->getFather()->setLocalTransform() = glm::rotate(e->getFather()->getLocalTransform(), force.z * t, glm::vec3(0, 0, 1));
		}
	}

	virtual void initialize()
	{}
};

#endif    //__EMPTY_SYSTEM_HPP__