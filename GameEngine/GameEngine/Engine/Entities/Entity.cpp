
#include "Entity.hh"
#include "Core/Engine.hh"
#include "EntityManager.h"
#include <limits>


Handle::Handle(unsigned int id, EntityManager *manager)
: _id(id), _manager(_manager)
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


size_t Entity::_currentId = 0;

Entity::Entity(Engine &engine) :
    PubSub(engine.getInstance<PubSub::Manager>()),
    _engine(engine),
	_id(_currentId++),
	_flags(0),
	_father(NULL)
{
}

Entity::~Entity()
{
	t_sonsList::iterator 	it = _sons.begin();

	while (it != _sons.end())
	{
		it->second->setFather(NULL);
		++it;
	}
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

Entity 				*Entity::getFather() const
{
	return (_father);
}

void 						Entity::setFather(Entity *father)
{
	_flags |= HAS_MOVED;
	_father = father;
}

void 						Entity::addSon(SmartPointer<Entity> const &son)
{
	t_sonsList::iterator 	it;
	size_t 					sonId = son->getId();

	if ((it = _sons.find(sonId)) == _sons.end())
	{
		son->setFather(this);
		_sons[sonId] = son;
	}
}

void 						Entity::removeSon(size_t sonId)
{
	t_sonsList::iterator 	it;

  	if ((it = _sons.find(sonId)) != _sons.end())
	{
		it->second->setFather(NULL);
		_sons.erase(it);
	}
}

SmartPointer<Entity> 	Entity::getSon(size_t sonId)
{
	t_sonsList::iterator 	it;

	if ((it = _sons.find(sonId)) != _sons.end())
		return (it->second);
	return (SmartPointer<Entity>(NULL));
}

SmartPointer<Entity> 	Entity::getSonRec(size_t sonId)
{
	t_sonsList::iterator	e = _sons.begin();

	while (e != _sons.end())
	{
		SmartPointer<Entity>	res;

		res = e->second->getSon(sonId);
		if (res != SmartPointer<Entity>(NULL))
			return (res);
		else
			e->second->getSonRec(sonId);
		++e;
	}
	return (SmartPointer<Entity>(NULL));
}

SmartPointer<Entity::t_EntityList> 	Entity::getSonsByFlags(size_t flags, GetFlags op)
{
	SmartPointer<Entity::t_EntityList>	sons = new Entity::t_EntityList;
	t_sonsList::iterator				e = _sons.begin();

	if (op == GetFlags::ONLY_THIS_FLAGS)
	{
		while (e != _sons.end())
		{
			if (((e->second->getFlags() & flags) == flags) &&
				((e->second->getFlags() | flags) == flags))
				sons->push_back(e->second);
			++e;
		}
	}
	else if (op == GetFlags::THIS_FLAGS)
	{
		while (e != _sons.end())
		{
			if ((e->second->getFlags() & flags) == flags)
				sons->push_back(e->second);
			++e;
		}
	}
	else if (op == GetFlags::NOT_THIS_FLAGS)
	{
		while (e != _sons.end())
		{
			if ((e->second->getFlags() & flags) == 0)
				sons->push_back(e->second);
			++e;
		}
	}
	return (sons);
}

Entity::t_sonsList::iterator 		Entity::getSonsBegin()
{
	return (_sons.begin());
}

Entity::t_sonsList::iterator 		Entity::getSonsEnd()
{
	return (_sons.end());
}

Barcode                       &Entity::getCode()
{
	return _code;
}

bool Entity::hasComponent(unsigned int componentId) const
{
		return _code.isSet(componentId);
}