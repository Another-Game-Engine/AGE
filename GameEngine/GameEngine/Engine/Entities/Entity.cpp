#include <Entities\Entity.hh>
#include <Entities\EntityData.hh>
#include <Core/Engine.hh>
#include <Components/Component.hh>
#include <limits>
#include <Core/AScene.hh>

Entity::Entity(unsigned int id, AScene *manager)
: _id(id), _manager(manager), _version(0)
{}

Entity::~Entity()
{}

Entity::Entity(Entity &&o)
{
	_id = std::move(o._id);
	_version = std::move(o._version);
	_manager = std::move(o._manager);
}

const unsigned int Entity::getId() const
{
	return _id;
}

const unsigned short Entity::getVersion() const
{
	return _version;
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
	return (_id == o._id && _version == o._version);
}

bool Entity::operator!=(const Entity &o) const
{
	return (_id != o._id || _version != o._version);
}

Entity::Entity(const Entity &o)
{
	_id = o._id;
	_version = o._version;
	_manager = o._manager;
}

Entity &Entity::operator=(const Entity &o)
{
	_id = o._id;
	_version = o._version;
	_manager = o._manager;
	return *this;
}