#ifndef   __BULLET_COLLISION_MANAGER_HPP__
#define   __BULLET_COLLISION_MANAGER_HPP__

#define __BT_SKIP_UINT64_H 1

#include <btBulletDynamicsCommon.h>
#include <BulletMultiThreaded/SpuGatheringCollisionDispatcher.h>
#include <BulletMultiThreaded/PlatformDefinitions.h>

#include "BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btSimulationIslandManager.h"

#include <BulletMultiThreaded/Win32ThreadSupport.h>
#include <BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h>

#include <BulletMultiThreaded/btParallelConstraintSolver.h>
#include <BulletMultiThreaded/SequentialThreadSupport.h>

#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

#include <Utils/Dependency.hpp>
#include <Physic/Utils/BtConversion.hpp>
#include <Entities/Entity.hh>
#include <Entities/EntityData.hh>

static btThreadSupportInterface *createSolverThreadSupport(int maxNumThreads)
{
	Win32ThreadSupport::Win32ThreadConstructionInfo threadConstructionInfo("solverThreads",SolverThreadFunc,SolverlsMemoryFunc,maxNumThreads);
	Win32ThreadSupport* threadSupport = new Win32ThreadSupport(threadConstructionInfo);
	threadSupport->startSPU();
	return threadSupport;
}

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
	int maxNumOutstandingTasks = 4;

	SequentialThreadSupport::SequentialThreadConstructionInfo colCI("collision", processCollisionTask, createCollisionLocalStoreMemory);
	auto m_threadSupportCollision = new SequentialThreadSupport(colCI);

	_dispatcher = new	SpuGatheringCollisionDispatcher(m_threadSupportCollision, maxNumOutstandingTasks, _collisionConfiguration);

	btVector3 worldAabbMin(-1000,-1000,-1000);
	btVector3 worldAabbMax(1000,1000,1000);
	const int maxProxies = 32766;
	const int maxOverlap = 65535;
	_broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);


	if (init) // init is false when called by Dynamic World
	{
		_world = new btCollisionWorld(_dispatcher, _broadphase, _collisionConfiguration);
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
			for (int it = 0, mit = raycastCallback.m_collisionObjects.size(); it < mit; ++it)
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
	btCollisionConfiguration *_collisionConfiguration;
};

#endif    //__BULLET_COLLISION_MANAGER_HPP__