
#include "Entity.hh"
#include "Core/Engine.hh"
#include "EntityManager.h"
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
	if (_id == max)
		return nullptr;
	return _manager->get(*this);
}

bool Handle::operator<(const Handle &o) const
{
	return _id < o._id;
}

size_t Entity::_currentId = 0;

Entity::Entity(Engine &engine) :
    PubSub(engine.getInstance<PubSub::Manager>()),
    _engine(engine),
	_id(_currentId++),
	_flags(0),
	_father(engine.getInstance<EntityManager>().getRoot())
{
}

Entity::~Entity()
{
	auto root = _engine.getInstance<EntityManager>().getRoot();
	for (auto e : _sons)
	{
		e->setFather(root);
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

const Handle 				&Entity::getFather() const
{
	return _father;
}

void 						Entity::setFather(Handle &father)
{
	_flags |= HAS_MOVED;
	_father = father;
}

void 						Entity::addSon(Handle &son)
{
	t_sonsList::iterator 	it;

	_sons.insert(son);
}

void 						Entity::removeSon(Handle &son)
{
	_sons.erase(son);
}

SmartPointer<Entity::t_EntityList> 	Entity::getSonsByFlags(size_t flags, GetFlags op)
{
	SmartPointer<Entity::t_EntityList>	sons = new Entity::t_EntityList;

	if (op == GetFlags::ONLY_THIS_FLAGS)
	{
		for (auto e : _sons)
		{
			if (((e->getFlags() & flags) == flags) &&
				((e->getFlags() | flags) == flags))
				sons->push_back(e);
		}
	}
	else if (op == GetFlags::THIS_FLAGS)
	{
		for (auto e : _sons)
		{
			if ((e->getFlags() & flags) == flags)
				sons->push_back(e);
		}
	}
	else if (op == GetFlags::NOT_THIS_FLAGS)
	{
		for (auto e : _sons)
		{
			if ((e->getFlags() & flags) == 0)
				sons->push_back(e);
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