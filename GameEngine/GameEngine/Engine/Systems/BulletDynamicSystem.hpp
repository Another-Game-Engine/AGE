#ifndef __BULLET_DYNAMIC_SYSTEM_HPP__
# define __BULLET_DYNAMIC_SYSTEM_HPP__

#include <Physic/Utils/BtConversion.hpp>
#include <Systems/System.h>
#include <Components/RigidBody.hpp>
#include <Physic/BulletDynamicManager.hpp>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>
#include <Components/CollisionBody.hpp>
#include <Entities/EntityFlags.hh>

#include <future>

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
//		auto &ff = _filter1.createFilterFast();
//
//static auto fn = std::function<void()>([&](){
//			auto b = ff.getComponent<Component::RigidBody>();
//			auto e = ff.getEntity();
//			if (b->getBody().isStaticOrKinematicObject())
//			{
//				btTransform transform;
//				
//				auto &entityTransform = _scene.lock()->getGlobalTransform(e);
//				glm::vec3 position = posFromMat4(entityTransform);
//				glm::vec3 scale = scaleFromMat4(entityTransform);
//				glm::vec3 rot = rotFromMat4(entityTransform, true);
//				transform.setIdentity();
//				transform.setOrigin(convertGLMVectorToBullet(position));
//				transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));
//
//				b->getBody().setWorldTransform(transform);
//				b->getShape().setLocalScaling(convertGLMVectorToBullet(scale));
//			}
//			else
//			{
//				btMotionState &state = b->getMotionState();
//				auto &shape = b->getShape();
//
//				btTransform wt;
//				state.getWorldTransform(wt);
//
//				glm::mat4 ATTRIBUTE_ALIGNED16(worldTrans);
//				wt.getOpenGLMatrix(glm::value_ptr(worldTrans));
//				worldTrans = glm::scale(worldTrans, convertBulletVectorToGLM(shape.getLocalScaling()));
//				scene->setLocalTransform(e, worldTrans);
//			}
//});
//
//		ff.forEach(fn);

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
				// TODO
				//if (e.flags & EntityData::HAS_MOVED)
				if (e.getFlags() & Flags::HasMoved)
				{
					//updateStatic(e);
					//e.setFlags() ^ Flags::HasMoved;
				}

				updateDynamic(e);
			}

		}
	}

	void updateCollisionBody(Entity &e)
	{
		btTransform transform;
		auto &entityTransform = _scene.lock()->getGlobalTransform(e);
		glm::vec3 position = posFromMat4(entityTransform);
		glm::vec3 scale = scaleFromMat4(entityTransform);
		glm::vec3 rot = rotFromMat4(entityTransform, true);
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
		
		auto &entityTransform = _scene.lock()->getGlobalTransform(e);
		glm::vec3 position = posFromMat4(entityTransform);
		glm::vec3 scale = scaleFromMat4(entityTransform);
		glm::vec3 rot = rotFromMat4(entityTransform, true);
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
		auto &shape = _scene.lock()->getComponent<Component::RigidBody>(e)->getShape();

		btTransform wt;
		state.getWorldTransform(wt);

		glm::mat4 ATTRIBUTE_ALIGNED16(worldTrans);
		wt.getOpenGLMatrix(glm::value_ptr(worldTrans));
		worldTrans = glm::scale(worldTrans, convertBulletVectorToGLM(shape.getLocalScaling()));
		_scene.lock()->setLocalTransform(e, worldTrans);
	}

	virtual bool initialize()
	{
		_filter1.requireComponent<Component::RigidBody>();
		_filter2.requireComponent<Component::CollisionBody>();
		return true;
	}
};

#endif //__BULLET_DYNAMIC_SYSTEM_HPP__
