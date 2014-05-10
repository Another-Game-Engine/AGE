#ifndef   __BULLET_COLLISION_SYSTEM_HPP__
# define  __BULLET_COLLISION_SYSTEM_HPP__

#include <Physic/Utils/BtConversion.hpp>
#include <Systems/System.h>
#include <Components/CollisionBody.hpp>
#include <Entities/EntityData.hh>
#include <Physic/BulletCollisionManager.hpp>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>

class BulletCollisionSystem : public System
{
public:
	BulletCollisionSystem(std::weak_ptr<AScene> scene) : System(scene)
		, _manager(scene.lock()->getInstance<BulletCollisionManager>())
		, _filter(scene)
	{
		_name = "bullet_collision_system";
	}
	virtual ~BulletCollisionSystem(){}
private:
	std::shared_ptr<BulletCollisionManager> _manager;
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
		_manager->getWorld()->performDiscreteCollisionDetection();
	}

	virtual bool initialize()
	{
		_filter.requireComponent<Component::CollisionBody>();
		return true;
	}
};

#endif    //__BULLET_COLLISION_SYSTEM_HPP__