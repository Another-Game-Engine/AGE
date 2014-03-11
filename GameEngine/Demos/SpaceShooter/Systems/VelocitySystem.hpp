#ifndef   __VELOCITY_SYSTEM_HPP__
# define  __VELOCITY_SYSTEM_HPP__

#include <Systems/System.h>
#include <Entities/EntityData.hh>
#include <Core/Engine.hh>
#include <Components/VelocityComponent.hpp>
#include <glm/gtc/matrix_transform.hpp>


class VelocitySystem : public System
{
public:
	VelocitySystem(std::weak_ptr<AScene> scene)
		: System(scene)
		, _filter(scene)
	{}

	virtual ~VelocitySystem(){}
private:

	EntityFilter _filter;

	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{} 

	virtual void mainUpdate(double time)
	{
		auto totalTime = _scene.lock()->getInstance<Timer>()->getElapsed();
		for (auto e : _filter.getCollection())
		{
			auto velocity = e->getComponent<Component::Velocity>();
			auto t = e->getLocalTransform();
			t = glm::translate(t, velocity->compute(time, totalTime));
			t = glm::scale(t, glm::vec3(1.001, 1.001, 1.001));
			e->setLocalTransform(t);
		}
	}

	virtual void initialize()
	{
		_filter.requireComponent<Component::Velocity>();
	}
};

#endif    //__VELOCITY_SYSTEM_HPP__