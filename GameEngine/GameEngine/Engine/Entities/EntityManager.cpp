#include <limits>
#include "EntityManager.h"
#include "Entity.hh"


EntityManager::EntityManager(Engine *engine) :
_engine(*engine)
{	
}

EntityManager::~EntityManager()
{}

Handle &EntityManager::createEntity()
{
	if (_free.empty())
	{
		_pool.push_back(Entity(_engine));
		_pool.back().setHandle(Handle(_pool.size() - 1, this));
		_free.push(_pool.size() - 1);
	}
	unsigned int index = _free.back();
	_free.pop();
	return _pool[index].getHandle();
}

void EntityManager::destroy(const Handle &h)
{
	_free.push(h.getId());
}

Entity *EntityManager::get(const Handle &h)
{
	if (h.getId() >= _pool.size())
		return nullptr;
	return &_pool[h.getId()];
}
