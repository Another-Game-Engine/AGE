#ifndef   __BULLET_COLLISION_MANAGER_HPP__
#define   __BULLET_COLLISION_MANAGER_HPP__

#define __BT_SKIP_UINT64_H 1

#include <btBulletDynamicsCommon.h>
#include <BulletMultiThreaded/SpuGatheringCollisionDispatcher.h>
#include <BulletMultiThreaded/PlatformDefinitions.h>


#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

#include <Utils/Dependency.hpp>
#include <Physic/Utils/BtConversion.hpp>
#include <Entities/Entity.hh>

#include <set>

#include <memory>


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
		btDefaultCollisionConstructionInfo cci;
		cci.m_defaultMaxPersistentManifoldPoolSize = 32768;
		btCollisionConfiguration *_collisionConfiguration = new btDefaultCollisionConfiguration(cci);

		_dispatcher = new btCollisionDispatcher(_collisionConfiguration);

		btVector3 worldAabbMin(-1000, -1000, -1000);
		btVector3 worldAabbMax(1000, 1000, 1000);
		const int maxProxies = 32766;
		const int maxOverlap = 65535;
		_broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);


		if (init) // init is false when called by Dynamic World
		{
			_world = std::make_shared<btCollisionWorld>(_dispatcher, _broadphase, _collisionConfiguration);
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
		{
			delete _broadphase;
			_broadphase = nullptr;
		}
		if (_dispatcher)
		{
			delete _dispatcher;
			_dispatcher = nullptr;
		}
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
			for (int it = 0, mit = raycastCallback.m_collisionObjects.size(); it < mit; ++it)
			{
				r.insert(*static_cast<Entity*>(raycastCallback.m_collisionObjects.at(it)->getUserPointer()));
			}
		}
		return std::move(r);
	}

	inline btCollisionWorld *getWorld() const { return _world.get(); }
protected:
	std::shared_ptr<btCollisionWorld> _world;
	btCollisionDispatcher *_dispatcher;
	btBroadphaseInterface *_broadphase;
	btCollisionConfiguration *_collisionConfiguration;
};

#endif    //__BULLET_COLLISION_MANAGER_HPP__