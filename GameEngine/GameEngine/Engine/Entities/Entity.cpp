#include <Entities\Entity.hh>
#include <Entities\EntityData.hh>
#include <Core/Engine.hh>
#include <Components/Component.hh>
#include <limits>
#include <Core/AScene.hh>

Entity::Entity(unsigned int id, AScene *manager)
: _id(id), _manager(manager)
{}

Entity::~Entity()
{}


const unsigned int Entity::getId() const
{
	return _id;
}

EntityData *Entity::operator->()
{
	return _manager->get(*this);
}

EntityData *Entity::get() const
{
	static unsigned int max = std::numeric_limits<unsigned int>::max();
	if (!_manager || _id == max)
		return nullptr;
	return _manager->get(*this);
}

bool Entity::operator<(const Entity &o) const
{
	return _id < o._id;
}

bool Entity::operator==(const Entity &o) const
{
	return _id == o._id;
}

Entity::Entity(const Entity &o)
{
	_id = o._id;
	_manager = o._manager;
}

Entity &Entity::operator=(const Entity &o)
{
	_id = o._id;
	_manager = o._manager;
	return *this;
}