#ifndef  __BULLET_MANAGER_HPP__
# define __BULLET_MANAGER_HPP__

#include <btBulletDynamicsCommon.h>

class BulletManager
{
private:
	btDefaultCollisionConfiguration *_collisionConfiguation;
	btCollisionDispatcher *_collisionDispatcher;
	btBroadphaseInterface *_broadphaseInterface;
	btSequentialImpulseConstraintSolver *_constraintSolver;
	btDiscreteDynamicsWorld *_world;
public:
	static void myTickCallback(btDynamicsWorld *world, btScalar timeStep) {
//		std::cout << "The world just ticked by " << timeStep << " seconds" << std::endl;
	}

	bool init()
	{

// And then somewhere after you construct the world:

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
		delete _world;
		delete _constraintSolver;
		delete _broadphaseInterface;
		delete _collisionDispatcher;
		delete _collisionConfiguation;
	}

	btDiscreteDynamicsWorld &getWorld()
	{
		return *_world;
	}
};

#endif   //__BULLET_MANAGER_HPP__