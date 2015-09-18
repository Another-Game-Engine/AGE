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

#include <Utils/Profiler.hpp>

namespace AGE
{
	class EntityFilter;
	struct ComponentBase;
	class ComponentManager;
	struct EntitySerializationInfos;

	class Entity
	{
	public:
		Entity();
		~Entity();
		Entity(const Entity &o);
		Entity(Entity &&o);
		Entity &operator=(const Entity &o);
		bool operator==(const Entity &o) const;
		bool operator!=(const Entity &o) const;
		bool operator<(const Entity &o) const;
		bool operator<=(const Entity &o) const;
		bool operator>(const Entity &o) const;
		bool operator>=(const Entity &o) const;
		EntityData *operator->() const { return ptr; }
		EntityData *operator->() { return ptr; }

		AGE::Link *getLinkPtr() const;
		AGE::Link *getLinkPtr();
		AGE::Entity *getPtr() const;
		AGE::Entity *getPtr();
		EntityData *getDataPtr() { return ptr; }

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

		inline bool isValid() const
		{
			return id != MAX_ENTITY_NUMBER && ptr != nullptr;
		}
	private:
		mutable EntityData *ptr;
		ENTITY_ID id;
		ENTITY_VERSION version;
		ENTITY_FLAGS flags;

		friend AScene;
		friend EntityFilter;
		friend ComponentManager;
		friend EntitySerializationInfos;
	};
}

namespace std
{
	template<>
	struct hash < AGE::Entity >
	{
		typedef AGE::Entity argument_type;
		typedef std::size_t result_type;

		result_type operator()(argument_type const& s) const
		{
			result_type res = 0;
			res = s.getId();
			return res;
		}
	};
}