#ifndef   __BULLET_COLLISION_MANAGER_HPP__
#define   __BULLET_COLLISION_MANAGER_HPP__

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <Utils/Dependency.hpp>
#include <Physic/Utils/BtConversion.hpp>
#include <Entities/Entity.hh>
#include <Entities/EntityData.hh>

class BulletCollisionManager : public Dependency
{
public:
	BulletCollisionManager()
		: _world(nullptr)
		, _dispatcher(nullptr)
		, _broadphase(nullptr)
	{}

	virtual bool init(bool init = true)
	{
		_dispatcher = new btCollisionDispatcher(&_defaultCollisionConfiguration);
		_broadphase = new btDbvtBroadphase();
		if (init) // init is false when called by Dynamic World
		{
			_world = new btCollisionWorld(_dispatcher, _broadphase, &_defaultCollisionConfiguration);
			btCollisionDispatcher * dispatcher = static_cast<btCollisionDispatcher*>(_world->getDispatcher());
			btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
		}
		return true;
	}

	virtual ~BulletCollisionManager()
	{
		uninit();
	}

	virtual void uninit()
	{
		if (_broadphase)
			delete _broadphase;
		if (_dispatcher)
			delete _dispatcher;
		if (_world)
			delete _world;
	}

	void addObject(btCollisionObject *object, int filterGroup, int collisionFilter)
	{
		_world->addCollisionObject(object, filterGroup, collisionFilter);
	}

	void removeObject(btCollisionObject *object)
	{
		_world->removeCollisionObject(object);
	}

	std::set<Entity> rayCast(const glm::vec3 &from, const glm::vec3 &to) const
	{
		std::set<Entity> r;
		btCollisionWorld::AllHitsRayResultCallback raycastCallback(convertGLMVectorToBullet(from), convertGLMVectorToBullet(to));
		raycastCallback.m_collisionFilterMask = btBroadphaseProxy::AllFilter;
		raycastCallback.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
		_world->rayTest(convertGLMVectorToBullet(from), convertGLMVectorToBullet(to), raycastCallback);
		if (raycastCallback.hasHit())
		{
			for (std::size_t it = 0, mit = static_cast<std::size_t>(raycastCallback.m_collisionObjects.size()); it < mit; ++it)
			{
				r.insert(*static_cast<Entity*>(raycastCallback.m_collisionObjects.at(it)->getUserPointer()));
			}
		}
		return std::move(r);
	}

	inline btCollisionWorld *getWorld() const { return _world; }
protected:
	btCollisionWorld *_world;
	btCollisionDispatcher *_dispatcher;
	btBroadphaseInterface *_broadphase;
	btDefaultCollisionConfiguration _defaultCollisionConfiguration;
};

#endif    //__BULLET_COLLISION_MANAGER_HPP__