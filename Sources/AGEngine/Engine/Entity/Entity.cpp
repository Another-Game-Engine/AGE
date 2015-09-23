#include "Entity.hh"
#include <Core/AScene.hh>
#include <Components/ComponentRegistrationManager.hpp>

namespace AGE
{

	Entity::Entity()
		: id(MAX_ENTITY_NUMBER)
		, version(0)
		, flags(0)
	{}

	Entity::~Entity()
	{}

	Entity::Entity(const Entity &o)
	{
		id = o.id;
		version = o.version;
		flags = o.flags;
		ptr = o.ptr;
	}

	Entity::Entity(Entity &&o)
	{
		id = std::move(o.id);
		version = std::move(o.version);
		flags = std::move(o.flags);
		ptr = o.ptr;
	}

	Entity &Entity::operator=(const Entity &o)
	{
		id = o.id;
		version = o.version;
		flags = o.flags;
		ptr = o.ptr;
		return *this;
	}

	bool Entity::operator==(const Entity &o) const
	{
		return version == o.version && id == o.id;
	}

	bool Entity::operator!=(const Entity &o) const
	{
		return !(version == o.version && id == o.id);
	}

	bool Entity::operator<(const Entity &o) const
	{
		return id < o.id;
	}

	bool Entity::operator<=(const Entity &o) const
	{
		return id <= o.id;
	}

	bool Entity::operator>(const Entity &o) const
	{
		return id > o.id;
	}

	bool Entity::operator>=(const Entity &o) const
	{
		return id >= o.id;
	}

	Entity *Entity::getPtr()
	{
		return &ptr->getEntity();
	}

	Entity *Entity::getPtr() const
	{
		return &ptr->getEntity();
	}

	AGE::Link *Entity::getLinkPtr()
	{
		return &ptr->getLink();
	}

	AGE::Link *Entity::getLinkPtr() const
	{
		return &ptr->getLink();
	}
}