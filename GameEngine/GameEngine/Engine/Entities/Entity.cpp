
#include "Entity.hh"

#include <limits>

size_t Entity::_currentId = 0;

Entity::Entity() :
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
			if ((e->second->getFlags() & flags == flags) &&
				(e->second->getFlags() | flags == flags))
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

void					Entity::addComponent(SmartPointer<Components::AComponent> const &component)
{
	_components[component->getName()] = component;
	component->setFather(this);
}

bool					Entity::removeComponent(std::string const &name)
{
	std::map<std::string, SmartPointer<Components::AComponent> >::iterator	it;

	if ((it = _components.find(name)) == _components.end())
		return (false);
	it->second->setFather(NULL);
	_components.erase(it);
	return (true);
}

Entity::t_sonsList::iterator 		Entity::getSonsBegin()
{
	return (_sons.begin());
}

Entity::t_sonsList::iterator 		Entity::getSonsEnd()
{
	return (_sons.end());
}

Entity::t_ComponentsList::iterator 	Entity::getComponentsBegin()
{
	return (_components.begin());
}

Entity::t_ComponentsList::iterator 	Entity::getComponentsEnd()
{
	return (_components.end());
}
