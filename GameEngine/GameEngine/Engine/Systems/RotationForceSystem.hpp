#ifndef   __ROTATION_FORCE_SYSTEM_HPP__
# define  __ROTATION_FORCE_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/RotationForce.hpp>
#include <Entities/EntityData.hh>

class RotationForceSystem : public System
{
public:
	RotationForceSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _filter(std::move(scene))
	{
		_name = "rotation_force_system";
	}
	virtual ~RotationForceSystem(){}
private:
	EntityFilter _filter;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		float t = static_cast<float>(time);
		for (auto e : _filter.getCollection())
		{
			glm::vec3 force = e->getComponent<Component::RotationForce>()->getForce();
			e->rotate(force * glm::vec3(t));
			e->setLocalTransform(glm::rotate(e->getLocalTransform(), force.x * t, glm::vec3(1, 0, 0)));
			e->setLocalTransform(glm::rotate(e->getLocalTransform(), force.y * t, glm::vec3(0, 1, 0)));
			e->setLocalTransform(glm::rotate(e->getLocalTransform(), force.z * t, glm::vec3(0, 0, 1)));
		}
	}

	virtual bool initialize()
	{
		_filter.requireComponent<Component::RotationForce>();
		return true;
	}
};

#endif    //__ROTATION_FORCE_SYSTEM_HPP__