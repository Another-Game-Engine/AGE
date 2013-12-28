#ifndef   __BULLET_COLLISION_SYSTEM_HPP__
# define  __BULLET_COLLISION_SYSTEM_HPP__

#include <Utils/BtConversion.hpp>
#include <Systems/System.h>
#include <Components/CollisionBody.hpp>
#include <Entities/Entity.hh>
#include <Managers/BulletCollisionManager.hpp>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>

class BulletCollisionSystem : public System
{
public:
	BulletCollisionSystem(Engine &engine) : System(engine)
		, _manager(engine.getInstance<BulletCollisionManager>())
	{}
	virtual ~BulletCollisionSystem(){}
private:
	BulletCollisionManager &_manager;
	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		// UPDATE POSITION OF CollisionBodies
		for (auto e : _collection)
		{
			btTransform transform;
			glm::vec3 position = posFromMat4(e->getGlobalTransform());
			glm::vec3 scale = scaleFromMat4(e->getGlobalTransform());
			glm::vec3 rot = rotFromMat4(e->getGlobalTransform(), true);
			transform.setIdentity();
			transform.setOrigin(convertGLMVectorToBullet(position));
			transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));

			auto c = e->getComponent<Component::CollisionBody>();
			c->getBody().setWorldTransform(transform);
			c->getShape().setLocalScaling(convertGLMVectorToBullet(scale));
		}
		// PERFORM COLLISION CHECK
		_manager.getWorld()->performDiscreteCollisionDetection();
	}

	virtual void initialize()
	{
		require<Component::CollisionBody>();
	}
};

#endif    //__BULLET_COLLISION_SYSTEM_HPP__