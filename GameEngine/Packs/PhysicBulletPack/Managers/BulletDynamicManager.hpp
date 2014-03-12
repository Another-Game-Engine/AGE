#ifndef  __BULLET_MANAGER_HPP__
# define __BULLET_MANAGER_HPP__

#include <Managers/BulletCollisionManager.hpp>

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
		_constraintSolver = new btSequentialImpulseConstraintSolver();
		// DISGUSTING !! LEAK !!!
		btVector3 worldMin(-1000, -1000, -1000);
		btVector3 worldMax(1000, 1000, 1000);
		btAxisSweep3* sweepBP = new btAxisSweep3(worldMin, worldMax);
		static auto test = sweepBP;

		_world = new btDiscreteDynamicsWorld(_dispatcher, test, _constraintSolver, &_defaultCollisionConfiguration);
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