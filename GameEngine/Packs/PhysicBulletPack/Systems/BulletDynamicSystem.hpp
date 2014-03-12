#ifndef   __BULLET_DYNAMIC_SYSTEM_HPP__
# define  __BULLET_DYNAMIC_SYSTEM_HPP__

#include <Utils/BtConversion.hpp>
#include <Systems/System.h>
#include <Components/RigidBody.hpp>
#include <Entities/EntityData.hh>
#include <Managers/BulletDynamicManager.hpp>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>


class BulletDynamicSystem : public System
{
public:
	BulletDynamicSystem(std::weak_ptr<AScene> scene) : System(scene)
		, _manager(nullptr)
		, _filter(scene)
	{
		_name = "bullet_dynamic_system";
		_manager = std::dynamic_pointer_cast<BulletDynamicManager>(_scene.lock()->getInstance<BulletCollisionManager>());
	}
	virtual ~BulletDynamicSystem(){}
private:
	std::shared_ptr<BulletDynamicManager> _manager;
	EntityFilter _filter;

	virtual void updateBegin(double time)
	{
		_manager->getWorld()->stepSimulation(static_cast<btScalar>(time), 10);
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		for (auto e : _filter.getCollection())
		{
			auto a = e;
			auto b = e->getComponent<Component::RigidBody>();
			if (e->getComponent<Component::RigidBody>()->getBody().isStaticOrKinematicObject())
				updateStatic(e);
			else
				updateDynamic(e);

		}
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

		auto c = e->getComponent<Component::RigidBody>();
		c->getBody().setWorldTransform(transform);
		c->getShape().setLocalScaling(convertGLMVectorToBullet(scale));
	}

	void updateDynamic(Entity &e)
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
		e->setLocalTransform(m);
	}

	virtual void initialize()
	{
		_filter.requireComponent<Component::RigidBody>();
	}
};

#endif    //__BULLET_DYNAMIC_SYSTEM_HPP__