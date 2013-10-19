
#include "AEntity.hh"

#include <limits>

size_t AEntity::_currentId = 0;

AEntity::AEntity() :
	_id(_currentId++),
	_flags(0),
	_father(NULL)
{
}

AEntity::~AEntity()
{
	t_sonsList::iterator 	it = _sons.begin();

	while (it != _sons.end())
	{
		it->second->setFather(NULL);
		++it;
	}
}

glm::mat4 const  		&AEntity::getLocalTransform()
{
	return (_localTransform);
}

glm::mat4   			&AEntity::setLocalTransform()
{
	_flags |= HAS_MOVED;
	return (_localTransform);
}

glm::mat4 const			&AEntity::getGlobalTransform() const
{
	return (_globalTransform);
}

void 					AEntity::computeGlobalTransform(glm::mat4 const &fatherTransform)
{
	_globalTransform = fatherTransform * _localTransform;
}

size_t 					AEntity::getId() const
{
	return (_id);
}

size_t 					AEntity::getFlags() const
{
	return (_flags);
}

void 					AEntity::setFlags(size_t flags)
{
	_flags = flags;
}

void 					AEntity::addFlags(size_t flags)
{
	_flags |= flags;
}

void 					AEntity::removeFlags(size_t flags)
{
	flags &= _flags;
	_flags ^= flags;
}

AEntity 				*AEntity::getFather() const
{
	return (_father);
}

void 						AEntity::setFather(AEntity *father)
{
	_flags |= HAS_MOVED;
	_father = father;
}

void 						AEntity::addSon(SmartPointer<AEntity> const &son)
{
	t_sonsList::iterator 	it;
	size_t 					sonId = son->getId();

	if ((it = _sons.find(sonId)) == _sons.end())
	{
		son->setFather(this);
		_sons[sonId] = son;
	}
}

void 						AEntity::removeSon(size_t sonId)
{
	t_sonsList::iterator 	it;

  	if ((it = _sons.find(sonId)) != _sons.end())
	{
		it->second->setFather(NULL);
		_sons.erase(it);
	}
}

SmartPointer<AEntity> 	AEntity::getSon(size_t sonId)
{
	t_sonsList::iterator 	it;

	if ((it = _sons.find(sonId)) != _sons.end())
		return (it->second);
	return (SmartPointer<AEntity>(NULL));
}

SmartPointer<AEntity> 	AEntity::getSonRec(size_t sonId)
{
	for (auto &e : _sons)
	{
		SmartPointer<AEntity>	res;

		res = e.second->getSon(sonId);
		if (res != SmartPointer<AEntity>(NULL))
			return (res);
		else
			e.second->getSonRec(sonId);
	}
	return (SmartPointer<AEntity>(NULL));
}

SmartPointer<AEntity::t_EntityList> 	AEntity::getSonsByFlags(size_t flags, GetFlags op)
{
	SmartPointer<AEntity::t_EntityList>	sons = new AEntity::t_EntityList;

	if (op == GetFlags::ONLY_THIS_FLAGS)
	{
		for (auto &e : _sons)
		{
			if ((e.second->getFlags() & flags == flags) &&
				(e.second->getFlags() | flags == flags))
				sons->push_back(e.second);
		}
	}
	else if (op == GetFlags::THIS_FLAGS)
	{
		for (auto &e : _sons)
		{
			if ((e.second->getFlags() & flags) == flags)
				sons->push_back(e.second);
		}
	}
	else if (op == GetFlags::NOT_THIS_FLAGS)
	{
		for (auto &e : _sons)
		{
			if ((e.second->getFlags() & flags) == 0)
				sons->push_back(e.second);
		}
	}
	return (sons);
}

void					AEntity::addComponent(SmartPointer<Components::AComponent> const &component)
{
	_components[component->getName()] = component;
	component->setFather(this);
}

bool					AEntity::removeComponent(std::string const &name)
{
	std::map<std::string, SmartPointer<Components::AComponent> >::iterator	it;

	if ((it = _components.find(name)) == _components.end())
		return (false);
	it->second->setFather(NULL);
	_components.erase(it);
	return (true);
}

AEntity::t_sonsList::iterator 		AEntity::getSonsBegin()
{
	return (_sons.begin());
}

AEntity::t_sonsList::iterator 		AEntity::getSonsEnd()
{
	return (_sons.end());
}