#ifndef   __BULLET_COLLISION_MANAGER_HPP__
#define   __BULLET_COLLISION_MANAGER_HPP__

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

class BulletCollisionManager
{
public:
	BulletCollisionManager()
		: _world(nullptr)
		, _dispatcher(nullptr)
		, _broadphase(nullptr)
	{}

	virtual bool init()
	{
		_dispatcher = new btCollisionDispatcher(&_defaultCollisionConfiguration);
		if (!_dispatcher)
			return false;
		_broadphase = new btDbvtBroadphase();
		if (!_broadphase)
			return false;
		_world = new btCollisionWorld(_dispatcher, _broadphase, &_defaultCollisionConfiguration);
		if (!_world)
			return false;
		btCollisionDispatcher * dispatcher = static_cast<btCollisionDispatcher*>(_world->getDispatcher());
		btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
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
protected:
	btCollisionWorld *_world;
	btCollisionDispatcher *_dispatcher;
	btBroadphaseInterface *_broadphase;
	btDefaultCollisionConfiguration _defaultCollisionConfiguration;
};

#endif    //__BULLET_COLLISION_MANAGER_HPP__