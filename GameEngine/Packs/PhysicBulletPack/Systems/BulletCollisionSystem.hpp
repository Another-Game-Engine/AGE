#ifndef   __BULLET_COLLISION_SYSTEM_HPP__
# define  __BULLET_COLLISION_SYSTEM_HPP__

#include <Utils/BtConversion.hpp>
#include <Systems/System.h>
#include <Components/CollisionBody.hpp>
#include <Entities/EntityData.hh>
#include <Managers/BulletCollisionManager.hpp>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>

class BulletCollisionSystem : public System
{
public:
	BulletCollisionSystem(AScene *scene) : System(scene)
		, _manager(scene->getEngine().getInstance<BulletCollisionManager>())
		, _filter(scene)
	{}
	virtual ~BulletCollisionSystem(){}
private:
	BulletCollisionManager &_manager;
	EntityFilter _filter;

	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		// UPDATE POSITION OF CollisionBodies
		for (auto e : _filter.getCollection())
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
		_filter.requireComponent<Component::CollisionBody>();
	}
};

#endif    //__BULLET_COLLISION_SYSTEM_HPP__