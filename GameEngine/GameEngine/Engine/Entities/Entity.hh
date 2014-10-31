#pragma once

#include <cstddef>
#include <utility>
#include <cstdint>
#include <bitset>

#include "EntityTypedef.hpp"

#include <cereal/cereal.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/bitset.hpp>
#include <cereal/types/vector.hpp>

#include <Core/Link.hpp>

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
		, active(false)
	{}

	~Entity()
	{}

	Entity(const Entity &o)
	{
		id = o.id;
		version = o.version;
		flags = o.flags;
		active = o.active;
	}

	Entity(Entity &&o)
	{
		id = std::move(o.id);
		version = std::move(o.version);
		flags = std::move(o.flags);
		active = o.active;
	}

	Entity &operator=(const Entity &o)
	{
		id = o.id;
		version = o.version;
		flags = o.flags;
		active = o.active;
		return *this;
	}

	bool operator==(const Entity &o) const
	{
		return version == o.version && id == o.id;
	}

	bool operator!=(const Entity &o) const
	{
		return !(version == o.version && id == o.id);
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

	inline const ENTITY_FLAGS &getFlags() const
	{
		return flags;
	}

	inline ENTITY_FLAGS &setFlags()
	{
		return flags;
	}

	inline bool isActive() const
	{
		return active;
	}

	inline void setActive(bool v)
	{
		active = v;
	}

private:
	ENTITY_ID id;
	ENTITY_VERSION version;
	ENTITY_FLAGS flags;
	bool active;

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

	bool operator==(const Barcode& o) const
	{
		return (code == o.code);
	}

	bool match(const Barcode &model) const
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

	inline bool hasTag(TAG_ID id) const
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

	inline bool hasComponent(COMPONENT_ID id) const
	{
		return code.test(id);
	}

	inline void reset()
	{
		code.reset();
	}

	template < typename Archive >
	void serialize(Archive &ar)
	{
		ar(
			cereal::make_nvp("bitset", code)
			);
	}
private:
	std::bitset<MAX_CPT_NUMBER + MAX_TAG_NUMBER> code;

	friend AScene;
	friend AComponentManager;
};

class EntityData
{
public:
	const Entity &getEntity() const { return entity; }
	const Barcode &getBarcode() const { return barcode; }
	const AGE::Link &getLink() const { return link; }
private:
	Entity entity;
	AGE::Link link;
	Barcode barcode;

public:
	friend AScene;
	friend AComponentManager;
	friend EntityFilter;
};