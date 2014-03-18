#ifndef  __BULLET_MANAGER_HPP__
# define __BULLET_MANAGER_HPP__

#include <Physic/BulletCollisionManager.hpp>

class BulletDynamicManager : public BulletCollisionManager
{
private:
	btSequentialImpulseConstraintSolver *_constraintSolver;
public:
	BulletDynamicManager()
		: BulletCollisionManager()
		, _constraintSolver(nullptr)
	{}

	virtual ~BulletDynamicManager()
	{
		uninit();
	}

	bool init()
	{
		if (!BulletCollisionManager::init(false))
			return false;

		btThreadSupportInterface *m_threadSupportSolver = nullptr;
		int maxNumOutstandingTasks = 4;

	m_threadSupportSolver = createSolverThreadSupport(maxNumOutstandingTasks);
	_constraintSolver = new btSequentialImpulseConstraintSolver();
	//this solver requires the contacts to be in a contiguous pool, so avoid dynamic allocation
	_dispatcher->setDispatcherFlags(btCollisionDispatcher::CD_DISABLE_CONTACTPOOL_DYNAMIC_ALLOCATION);


	btDiscreteDynamicsWorld* world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _constraintSolver, _collisionConfiguration);

	world->getSimulationIslandManager()->setSplitIslands(false);
	world->getSolverInfo().m_numIterations = 4;
	world->getSolverInfo().m_solverMode = SOLVER_SIMD + SOLVER_USE_WARMSTARTING;//+SOLVER_RANDMIZE_ORDER;

	world->getDispatchInfo().m_enableSPU = true;
	world->setGravity(btVector3(0, -10, 0));

	_world = world;
//		_world->getDispatchInfo().m_allowedCcdPenetration = 0.01f;
	static_cast<btDiscreteDynamicsWorld *>(_world)->setGravity(btVector3(0, -10, 0));
	return true;
	}

	virtual void uninit()
	{
		if (_constraintSolver)
			delete _constraintSolver;
	}
	inline btDynamicsWorld * getWorld() const { return static_cast<btDynamicsWorld *>(_world); }
};

#endif   //__BULLET_MANAGER_HPP__