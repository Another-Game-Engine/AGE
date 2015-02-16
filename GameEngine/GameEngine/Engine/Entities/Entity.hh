#pragma once

#include <cstddef>
#include <utility>
#include <cstdint>
#include <bitset>

#include "EntityTypedef.hpp"

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/bitset.hpp>
#include <cereal/types/vector.hpp>

#include <Core/Link.hpp>

namespace AGE
{
	class EntityData;
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


		const AGE::Link &getLink() const;
		AGE::Link &getLink();
		const AGE::Entity *getPtr() const;
		AGE::Entity *getPtr();
		AScene *getScene();

		ComponentBase *getComponent(ComponentType id);

		template <typename T>
		T *getComponent()
		{
			return static_cast<T*>(ptr->getComponent(Component<T>::getTypeId()));
		}

		template <typename T, typename... Args>
		T *addComponent(Args &&...args)
		{
			return ptr->addComponent<T>(args...);
		}

		void removeComponent(ComponentType id);

		template <typename T>
		void removeComponent()
		{
			ptr->removeComponent(Component<T>::getTypeId());
		}

		bool haveComponent(ComponentType id) const;
		void addComponentPtr(ComponentBase *cpt);

		template <typename T>
		bool haveComponent() const
		{
			return ptr->haveComponent(Component<T>::getTypeId());
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

		const std::vector<ComponentBase*> &getComponentList() const;
	private:
		ENTITY_ID id;
		ENTITY_VERSION version;
		ENTITY_FLAGS flags;
		EntityData *ptr;

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