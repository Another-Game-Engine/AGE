
#include "Entity.hh"
#include "Core/Engine.hh"
#include "EntityManager.h"
#include "Components/Component.hh"
#include "Handle.hh"
#include <limits>


size_t Entity::_currentId = 0;

Entity::Entity(Engine &engine) :
    PubSub(engine.getInstance<PubSub::Manager>()),
    _engine(engine),
	_id(_currentId++),
	_flags(0)
{
}

Entity::~Entity()
{
	_components.clear();
}

Handle &Entity::getHandle()
{
	return _handle;
}

void Entity::setHandle(Handle &handle)
{
	_handle = handle;
}


glm::mat4 const  		&Entity::getLocalTransform()
{
	return (_localTransform);
}

glm::mat4   			&Entity::setLocalTransform()
{
	_flags |= HAS_MOVED;
	return (_localTransform);
}

glm::mat4 const			&Entity::getGlobalTransform() const
{
	return (_globalTransform);
}

void 					Entity::computeGlobalTransform(glm::mat4 const &fatherTransform)
{
	_globalTransform = fatherTransform * _localTransform;
	_flags ^= HAS_MOVED;
}

size_t 					Entity::getId() const
{
	return (_id);
}

size_t 					Entity::getFlags() const
{
	return (_flags);
}

void 					Entity::setFlags(size_t flags)
{
	_flags = flags;
}

void 					Entity::addFlags(size_t flags)
{
	_flags |= flags;
}

void 					Entity::removeFlags(size_t flags)
{
	flags &= _flags;
	_flags ^= flags;
}

Barcode                       &Entity::getCode()
{
	return _code;
}

bool Entity::hasComponent(unsigned int componentId) const
{
		return _code.isSet(componentId);
}