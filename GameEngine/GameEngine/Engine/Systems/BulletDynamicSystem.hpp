#pragma once

#include <Physic/Utils/BtConversion.hpp>
#include <Systems/System.h>
#include <Components/RigidBody.hpp>
#include <Physic/BulletDynamicManager.hpp>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>
#include <Entities/EntityFlags.hh>

#include <future>

class BulletDynamicSystem : public System
{
public:
	BulletDynamicSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _manager(nullptr)
	{
		_name = "bullet_dynamic_system";
		_manager = dynamic_cast<BulletDynamicManager*>(_scene.lock()->getInstance<BulletCollisionManager>());
	}
	virtual ~BulletDynamicSystem(){}
private:
	BulletDynamicManager* _manager;

	virtual void updateBegin(double time)
	{
		_manager->getWorld()->stepSimulation(static_cast<btScalar>(time), 10);
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
	}

	virtual bool initialize()
	{
		return true;
	}
};
