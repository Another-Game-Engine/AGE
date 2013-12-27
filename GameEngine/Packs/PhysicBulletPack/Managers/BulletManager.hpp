#ifndef  __BULLET_MANAGER_HPP__
# define __BULLET_MANAGER_HPP__

#include <btBulletDynamicsCommon.h>

class BulletDynamicManager
{
private:
	btDefaultCollisionConfiguration *_collisionConfiguation;
	btCollisionDispatcher *_collisionDispatcher;
	btBroadphaseInterface *_broadphaseInterface;
	btSequentialImpulseConstraintSolver *_constraintSolver;
	btDiscreteDynamicsWorld *_world;
public:
	BulletDynamicManager()
	{}

	~BulletDynamicManager()
	{
		uninit();
	}

	static void myTickCallback(btDynamicsWorld *world, btScalar timeStep) {
	}

	bool init()
	{
		_collisionConfiguation = new btDefaultCollisionConfiguration();
		_collisionDispatcher = new btCollisionDispatcher(_collisionConfiguation);
		_broadphaseInterface = new btDbvtBroadphase();
		_constraintSolver = new btSequentialImpulseConstraintSolver();
		_world = new btDiscreteDynamicsWorld(_collisionDispatcher, _broadphaseInterface, _constraintSolver, _collisionConfiguation);
		_world->setGravity(btVector3(0, -10, 0));
		_world->setInternalTickCallback(myTickCallback);
		return true;
	}

	void uninit()
	{
		if (_world)
			delete _world;
		if (_constraintSolver)
			delete _constraintSolver;
		if (_broadphaseInterface)
			delete _broadphaseInterface;
		if (_collisionDispatcher)
			delete _collisionDispatcher;
		if (_collisionConfiguation)
			delete _collisionConfiguation;
	}

	btDiscreteDynamicsWorld &getWorld()
	{
		return *_world;
	}
};

#endif   //__BULLET_MANAGER_HPP__