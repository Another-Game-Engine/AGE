#ifndef   __VELOCITY_SYSTEM_HPP__
# define  __VELOCITY_SYSTEM_HPP__

#include <Systems/System.h>
#include <Entities/Entity.hh>
#include <Core/Engine.hh>
#include <Components/VelocityComponent.hpp>
#include <glm/gtc/matrix_transform.hpp>


class VelocitySystem : public System
{
public:
	VelocitySystem(Engine &engine) : System(engine)
	{}
	virtual ~VelocitySystem(){}
private:
	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{} 

	virtual void mainUpdate(double time)
	{
		auto totalTime = _engine.getInstance<Timer>().getElapsed();
		for (auto e : _collection)
		{
			auto velocity = e->getComponent<Component::Velocity>();
			auto lol = posFromMat4(e->getLocalTransform());
			auto lilou = velocity->compute(time, totalTime);
			auto crotte = posFromMat4(e->getLocalTransform()) + velocity->compute(time, totalTime);
			e->setLocalTransform() = glm::translate(e->getLocalTransform(), velocity->compute(time, totalTime));
			e->setLocalTransform() = glm::scale(e->getLocalTransform(), glm::vec3(1.001, 1.001, 1.001));
//			e->setLocalTransform() = glm::translate(e->getLocalTransform(), glm::vec3(0,-0.1,0) * (float)time);
		}
	}

	virtual void initialize()
	{
		require<Component::Velocity>();
	}
};

#endif    //__VELOCITY_SYSTEM_HPP__