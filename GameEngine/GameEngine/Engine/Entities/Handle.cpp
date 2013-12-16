#include "Handle.hh"
#include "Entity.hh"
#include "Core/Engine.hh"
#include "EntityManager.h"
#include "Components/Component.hh"
#include <limits>

Handle::Handle(unsigned int id, EntityManager *manager)
: _id(id), _manager(manager)
{}

Handle::~Handle()
{}


const unsigned int Handle::getId() const
{
	return _id;
}

Entity *Handle::operator->()
{
	return _manager->get(*this);
}

Entity *Handle::get() const
{
	static unsigned int max = std::numeric_limits<unsigned int>::max();
	if (!_manager || _id == max)
		return nullptr;
	return _manager->get(*this);
}

bool Handle::operator<(const Handle &o) const
{
	return _id < o._id;
}

bool Handle::operator==(const Handle &o) const
{
	return _id == o._id;
}

Handle::Handle(const Handle &o)
{
	_id = o._id;
	_manager = o._manager;
}

Handle &Handle::operator=(const Handle &o)
{
	_id = o._id;
	_manager = o._manager;
	return *this;
}