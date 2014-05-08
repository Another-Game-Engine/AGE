#pragma once

#include <cstddef>
#include <utility>
#include <cstdint>
#include <bitset>

#define MAX_TAG_NUMBER (32)
#define MAX_CPT_NUMBER (96)

typedef std::uint16_t ENTITY_ID;
typedef std::uint8_t  COMPONENT_ID;
typedef std::uint8_t  TAG_ID;
typedef std::uint8_t  ENTITY_VERSION;
typedef std::uint8_t  ENTITY_FLAGS;

#define MAX_ENTITY_NUMBER ((ENTITY_ID)(-1))

class AScene;
class AComponentManager;
class EntityFilter;

class Entity
{
public:
	Entity()
		: id(0)
		, version(0)
		, flags(0)
	{}

	~Entity()
	{}

	Entity(const Entity &o)
	{
		id = o.id;
		version = o.version;
		flags = o.flags;
	}

	Entity(Entity &&o)
	{
		id = std::move(o.id);
		version = std::move(o.version);
		flags = std::move(o.flags);
	}

	Entity &operator=(const Entity &o)
	{
		id = o.id;
		version = o.version;
		flags = o.flags;
		return *this;
	}

	bool operator==(const Entity &o) const
	{
		return version == o.version && id == o.id && flags == o.flags;
	}

	bool operator!=(const Entity &o) const
	{
		return !(version == o.version && id == o.id && flags == o.flags);
	}

	bool operator<(const Entity &o) const
	{
		return id < o.id;
	}

	bool operator<=(const Entity &o) const
	{
		return id <= o.id;
	}

	bool operator>(const Entity &o) const
	{
		return id > o.id;
	}

	bool operator>=(const Entity &o) const
	{
		return id >= o.id;
	}

	inline ENTITY_ID getId() const
	{
		return id;
	}

	inline ENTITY_VERSION getVersion() const
	{
		return version;
	}

	inline ENTITY_FLAGS getFlags() const
	{
		return flags;
	}

private:
	ENTITY_ID id;
	ENTITY_VERSION version;
	ENTITY_FLAGS flags;

	friend AScene;
	friend AComponentManager;
	friend EntityFilter;
};

class Barcode
{
public:
	Barcode()
		: code(0)
	{}

	~Barcode()
	{}

	Barcode &operator=(const Barcode& o)
	{
		code = o.code;
		return *this;
	}

	Barcode(const Barcode& o)
	{
		code = o.code;
	}

	Barcode(Barcode&& o)
	{
		code = std::move(o.code);
	}

	bool operator==(const Barcode& o)
	{
		return (code == o.code);
	}

	bool match(const Barcode &model)
	{
		return (code & model.code) == model.code;
	}

	inline void setTag(TAG_ID id)
	{
		code.set(id + MAX_CPT_NUMBER);
	}

	inline void unsetTag(TAG_ID id)
	{
		code.reset(id + MAX_CPT_NUMBER);
	}

	inline bool hasTag(TAG_ID id)
	{
		return code.test(id + MAX_CPT_NUMBER);
	}

	inline void setComponent(COMPONENT_ID id)
	{
		code.set(id);
	}

	inline void unsetComponent(COMPONENT_ID id)
	{
		code.reset(id);
	}

	inline bool hasComponent(COMPONENT_ID id)
	{
		return code.test(id);
	}

	inline void reset()
	{
		code.reset();
	}
private:
	std::bitset<MAX_CPT_NUMBER + MAX_TAG_NUMBER> code;

	friend AScene;
	friend AComponentManager;
};

class EntityData
{
public:
private:
	Entity entity;
	uint32_t crap;
	Barcode barcode;

public:
	friend AScene;
	friend AComponentManager;
	friend EntityFilter;
};