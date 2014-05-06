#pragma once

#include <cstddef>
#include <utility>

#include <cstdint>

#include "BitsetManipulation.hpp"
#include "EntityFlags.hh"

#define MAX_TAG_NUMBER (32)
#define MAX_CPT_NUMBER (64)

typedef std::uint64_t COMPONENTS_BARCODE;
typedef std::uint32_t TAGS_BARCODE;
typedef std::uint16_t ENTITY_ID;
typedef std::uint8_t  COMPONENT_ID;
typedef std::uint8_t  TAG_ID;

class Entity
{
public:
	COMPONENTS_BARCODE components;
	TAGS_BARCODE tags;
	ENTITY_ID id;
	std::uint8_t version;
	std::uint8_t flags;

	Entity()
		: components(0)
		, tags(0)
		, id(0)
		, version(0)
		, flags(0)
	{}

	~Entity()
	{}

	Entity(const Entity &o)
	{
		components = o.components;
		tags = o.tags;
		id = o.id;
		version = o.version;
		flags = o.flags;
	}

	Entity(Entity &&o)
	{
		components = std::move(o.components);
		tags = std::move(o.tags);
		id = std::move(o.id);
		version = std::move(o.version);
		flags = std::move(o.flags);
	}

	Entity &operator=(const Entity &o)
	{
		components = o.components;
		tags = o.tags;
		id = o.id;
		version = o.version;
		flags = o.flags;
		return *this;
	}

	bool operator==(const Entity &o)
	{
		return version == o.version && id == o.id;
	}

	bool operator<(const Entity &o)
	{
		return id < o.id;
	}

	bool operator<=(const Entity &o)
	{
		return id <= o.id;
	}

	bool operator>(const Entity &o)
	{
		return id > o.id;
	}

	bool operator>=(const Entity &o)
	{
		return id >= o.id;
	}

	void reset()
	{
		components = 0;
		tags = 0;
		++version;
		flags = 0;
	}

	void init(ENTITY_ID _id)
	{
		id = _id;
	}

	void setComponent(COMPONENT_ID cptId)
	{
		BitsetManipulation::set(components, cptId);
	}

	void unsetComponent(COMPONENT_ID cptId)
	{
		BitsetManipulation::unset(components, cptId);
	}

	// todo
	bool hasComponent(COMPONENT_ID cptId)
	{
		return BitsetManipulation::isSet(components, cptId);
	}

	void setTag(TAG_ID tagId)
	{
		BitsetManipulation::set(tags, tagId);
	}

	void unsetTag(TAG_ID tagId)
	{
		BitsetManipulation::unset(tags, tagId);
	}

	bool isTagged(TAG_ID tagId)
	{
		return BitsetManipulation::isSet(tags, tagId);
	}
};


//class Entity
//{
//public:
//	Entity(std::size_t id = 0, AScene *manager = nullptr);
//	~Entity();
//	Entity(Entity &&o);
//	const std::size_t getId() const;
//	const unsigned short getVersion() const;
//	EntityData *operator->();
//	EntityData *operator->() const;
//	EntityData *get() const;
//	bool operator<(const Entity &o) const;
//	bool operator==(const Entity &o) const;
//	bool operator!=(const Entity &o) const;
//	Entity(const Entity &o);
//	Entity &operator=(const Entity &o);
//private:
//	friend class AScene;
//	std::size_t _id;
//	AScene *_manager;
//	unsigned short _version;
//};