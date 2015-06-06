#pragma once

#include "EntityTypedef.hpp"
#include <Core/Link.hpp>
#include "Entities/Entity.hh"

namespace AGE
{
	class AScene;
	class EntityFilter;
	class Entity;
	struct ComponentBase;
	class ComponentManager;
	struct EntitySerializationInfos;

	class EntityData
	{
	public:
		EntityData(AScene *scene);
		const Entity &getEntity() const;
		Entity &getEntity();
		const AGE::Link &getLink() const;
		AGE::Link &getLink();
		AScene *getScene();
		ComponentBase *getComponent(ComponentType id);
		void addComponentPtr(ComponentBase *cpt);
		void copyComponent(ComponentBase *cpt);
		void removeComponent(ComponentType id);
		bool haveComponent(ComponentType id) const;

		template <typename T>
		inline T *getComponent()
		{
			return (T*)(getComponent(Component<T>::getTypeId()));
		}

		template <typename T>
		inline bool haveComponent() const
		{
			return haveComponent(Component<T>::getTypeId());
		}

		template <typename T>
		inline void removeComponent()
		{
			removeComponent(Component<T>::getTypeId());
		}


		template <typename T, typename... Args>
		T *addComponent(Args &&...args)
		{
			auto id = Component<T>::getTypeId();
			if (haveComponent(id))
				return static_cast<T*>(getComponent(id));
			if (components.size() <= id)
				components.resize(id + 1, nullptr);
			T *ptr = scene->createComponent<T>(entity);
			components[id] = ptr;
			if (!outOfContext)
			{
				scene->informFiltersComponentAddition(id, *this);
			}
			components[id]->reset();
			ptr->init(std::forward<Args>(args)...);
			return ptr;
		}

		const std::vector<ComponentBase*> &getComponentList() const
		{
			return components;
		}
	private:
		Entity entity;
		AGE::Link link;
		std::vector<ComponentBase*> components;
		AScene *scene;
		bool outOfContext;
	public:
		friend AScene;
		friend EntityFilter;
		friend ComponentManager;
		friend EntitySerializationInfos;
	};
}