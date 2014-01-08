#ifndef   __BULLET_DYNAMIC_SYSTEM_HPP__
# define  __BULLET_DYNAMIC_SYSTEM_HPP__

#include <Utils/BtConversion.hpp>
#include <Systems/System.h>
#include <Components/RigidBody.hpp>
#include <Entities/Entity.hh>
#include <Managers/BulletDynamicManager.hpp>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>


class BulletDynamicSystem : public System
{
public:
	BulletDynamicSystem(AScene *scene) : System(scene)
		, _manager(nullptr)
	{
		_manager = dynamic_cast<BulletDynamicManager*>(&_scene->getEngine().getInstance<BulletCollisionManager>());
		assert(_manager != nullptr);
	}
	virtual ~BulletDynamicSystem(){}
private:
	BulletDynamicManager *_manager;
	virtual void updateBegin(double time)
	{
		_manager->getWorld()->stepSimulation(time, 10);
		btDispatcher *dispatcher = _manager->getWorld()->getDispatcher();
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
			if (e->getComponent<Component::RigidBody>()->getBody().isStaticOrKinematicObject())
				updateStatic(e);
			else
				updateDynamic(e);

		}
	}

	void updateStatic(Handle &e)
	{
		btTransform transform;
		glm::vec3 position = posFromMat4(e->getGlobalTransform());
		glm::vec3 scale = scaleFromMat4(e->getGlobalTransform());
		glm::vec3 rot = rotFromMat4(e->getGlobalTransform(), true);
		transform.setIdentity();
		transform.setOrigin(convertGLMVectorToBullet(position));
		transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));

		auto c = e->getComponent<Component::RigidBody>();
		c->getBody().setWorldTransform(transform);
		c->getShape().setLocalScaling(convertGLMVectorToBullet(scale));
	}

	void updateDynamic(Handle &e)
	{
		btMotionState &state = e->getComponent<Component::RigidBody>()->getMotionState();
		glm::mat4 m;
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
	}

	virtual void initialize()
	{
		require<Component::RigidBody>();
	}
};

#endif    //__BULLET_DYNAMIC_SYSTEM_HPP__