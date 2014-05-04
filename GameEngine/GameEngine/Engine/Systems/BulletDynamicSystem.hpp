#ifndef   __BULLET_DYNAMIC_SYSTEM_HPP__
# define  __BULLET_DYNAMIC_SYSTEM_HPP__

#include <Physic/Utils/BtConversion.hpp>
#include <Systems/System.h>
#include <Components/RigidBody.hpp>
#include <Entities/EntityData.hh>
#include <Physic/BulletDynamicManager.hpp>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>
#include <Components/CollisionBody.hpp>


class BulletDynamicSystem : public System
{
public:
	BulletDynamicSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _manager(nullptr)
		, _filter1(std::move(scene))
		, _filter2(std::move(scene))
	{
		_name = "bullet_dynamic_system";
		_manager = std::dynamic_pointer_cast<BulletDynamicManager>(_scene.lock()->getInstance<BulletCollisionManager>());
	}
	virtual ~BulletDynamicSystem(){}
private:
	std::shared_ptr<BulletDynamicManager> _manager;
	EntityFilter _filter1;
	EntityFilter _filter2;

	virtual void updateBegin(double time)
	{
		_manager->getWorld()->stepSimulation(static_cast<btScalar>(time), 10);
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		auto scene = _scene.lock();
		for (auto e : _filter2.getCollection())
		{
			updateCollisionBody(e);
		}
		for (auto e : _filter1.getCollection())
		{
			auto a = e;
			auto b = scene->getComponent<Component::RigidBody>(e);
			if (scene->getComponent<Component::RigidBody>(e)->getBody().isStaticOrKinematicObject())
			{
				updateStatic(e);
			}
			else
			{
				if (e->getFlags() & EntityData::HAS_MOVED)
					updateStatic(e);
				updateDynamic(e);
			}

		}
	}

	void updateCollisionBody(Entity &e)
	{
		btTransform transform;
		glm::vec3 position = posFromMat4(e->getGlobalTransform());
		glm::vec3 scale = scaleFromMat4(e->getGlobalTransform());
		glm::vec3 rot = rotFromMat4(e->getGlobalTransform(), true);
		transform.setIdentity();
		transform.setOrigin(convertGLMVectorToBullet(position));
		transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));

		auto c = _scene.lock()->getComponent<Component::CollisionBody>(e);
		c->getBody().setWorldTransform(transform);
		c->getShape().setLocalScaling(convertGLMVectorToBullet(scale));
	}

	void updateStatic(Entity &e)
	{
		btTransform transform;
		glm::vec3 position = posFromMat4(e->getGlobalTransform());
		glm::vec3 scale = scaleFromMat4(e->getGlobalTransform());
		glm::vec3 rot = rotFromMat4(e->getGlobalTransform(), true);
		transform.setIdentity();
		transform.setOrigin(convertGLMVectorToBullet(position));
		transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));

		auto c = _scene.lock()->getComponent<Component::RigidBody>(e);
		c->getBody().setWorldTransform(transform);
		c->getShape().setLocalScaling(convertGLMVectorToBullet(scale));
	}

	void updateDynamic(Entity &e)
	{
		btMotionState &state = _scene.lock()->getComponent<Component::RigidBody>(e)->getMotionState();
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
		e->setLocalTransform(m);
	}

	virtual bool initialize()
	{
		_filter1.requireComponent<Component::RigidBody>();
		_filter2.requireComponent<Component::CollisionBody>();
		return true;
	}
};

#endif    //__BULLET_DYNAMIC_SYSTEM_HPP__