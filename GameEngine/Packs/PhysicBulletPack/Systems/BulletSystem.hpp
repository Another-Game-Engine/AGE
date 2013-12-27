#ifndef   __BULLET_SYSTEM_HPP__
# define  __BULLET_SYSTEM_HPP__

#include <Utils/BtConversion.hpp>
#include <Systems/System.h>
#include <Components/RigidBody.hpp>
#include <Entities/Entity.hh>
#include <Managers/BulletDynamicManager.hpp>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>


class BulletSystem : public System
{
public:
	BulletSystem(Engine &engine) : System(engine)
		, _manager(engine.getInstance<BulletDynamicManager>())
	{}
	virtual ~BulletSystem(){}
private:
	BulletDynamicManager &_manager;
	virtual void updateBegin(double time)
	{
		_manager.getWorld().stepSimulation(time,10);
		btDispatcher *dispatcher = _manager.getWorld().getDispatcher();
		unsigned int max = dispatcher->getNumManifolds();
		for (unsigned int i = 0; i < max; ++i)
		{
			btPersistentManifold *contact = dispatcher->getManifoldByIndexInternal(i);
			const btCollisionObject *oa = static_cast<const btCollisionObject*>(contact->getBody0());
			const btCollisionObject *ob = static_cast<const btCollisionObject*>(contact->getBody1());

			Handle h1 = *(static_cast<Handle*>(oa->getUserPointer()));
			Entity *e1 = h1.get();
			auto c1 = e1->addComponent<Component::Collision>();

			Handle h2 = *(static_cast<Handle*>(ob->getUserPointer()));
			Entity *e2 = h2.get();
			auto c2 = e2->addComponent<Component::Collision>();


			c1->addCollision(h2);
			c2->addCollision(h1); 
		}
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		for (auto e : _collection)
		{
			glm::mat4 m;
			btMotionState &state = e->getComponent<Component::RigidBody>()->getMotionState();
			btTransform trans;
			state.getWorldTransform(trans);
			glm::mat4 t = convertBulletTransformToGLM(trans);
			m = glm::translate(m, posFromMat4(t));
			glm::vec3 rot = rotFromMat4(t, false);
			m = glm::rotate(m, rot.x, glm::vec3(1, 0, 0));
			m = glm::rotate(m, rot.y, glm::vec3(0, 1, 0));
			m = glm::rotate(m, rot.z, glm::vec3(0, 0, 1));
			glm::vec3 scale = scaleFromMat4(e->getLocalTransform());
			m = glm::scale(m, scale);
			e->setLocalTransform() = m;
			e->removeComponent<Component::Collision>();
		}
	}

	virtual void initialize()
	{
		require<Component::RigidBody>();
	}
};

#endif    //__BULLET_SYSTEM_HPP__