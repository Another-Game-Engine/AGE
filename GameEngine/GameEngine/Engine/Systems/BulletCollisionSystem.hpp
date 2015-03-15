#ifndef   __BULLET_COLLISION_SYSTEM_HPP__
# define  __BULLET_COLLISION_SYSTEM_HPP__

#include <Physic/Utils/BtConversion.hpp>
#include <Systems/System.h>
#include <Components/CollisionBody.hpp>
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

	virtual void updateBegin(float time)
	{
	}

	virtual void updateEnd(float time)
	{}

	virtual void mainUpdate(float time)
	{
		// UPDATE POSITION OF CollisionBodies
		auto scene = _scene.lock();
		for (auto e : _filter.getCollection())
		{
			btTransform transform;
			auto &globalTrans = e.getLink().getGlobalTransform();
			glm::vec3 position = posFromMat4(globalTrans);
			glm::vec3 scale = scaleFromMat4(globalTrans);
			glm::vec3 rot = rotFromMat4(globalTrans, true);
			transform.setIdentity();
			transform.setOrigin(convertGLMVectorToBullet(position));
			transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));

			auto c = scene->getComponent<Component::CollisionBody>(e);
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