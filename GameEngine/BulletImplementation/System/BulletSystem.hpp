#ifndef   __BULLET_SYSTEM_HPP__
# define  __BULLET_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Systems/System.h>
#include <Component/RigidBody.hpp>
#include <Entities/Entity.hh>
#include "../BulletManager.hpp"
#include <Core/Engine.hh>


class BulletSystem : public System
{
public:
	BulletSystem(Engine &engine) : System(engine)
		, _manager(engine.getInstance<BulletManager>())
	{}
	virtual ~BulletSystem(){}
private:
	BulletManager &_manager;
	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		_manager.getWorld().stepSimulation(time,10);
		for (auto e : _collection)
		{
			btTransform t;
			e->getComponent<Component::RigidBody>()->getMotionState().getWorldTransform(t);
			glm::mat4 m = convertBulletTransformToGLM(t);
			glm::vec3 s = convertBulletVectorToGLM(e->getComponent<Component::RigidBody>()->getShape().getLocalScaling());

			m = glm::scale(m, s);
			e->setLocalTransform() = m;
		}
	}

	virtual void initialize()
	{
		require<Component::RigidBody>();
	}
};

#endif    //__BULLET_SYSTEM_HPP__