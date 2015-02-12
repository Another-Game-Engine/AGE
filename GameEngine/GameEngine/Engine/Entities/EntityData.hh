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


	class EntityData
	{
	public:
		const Entity &getEntity() const;
		Entity &getEntity();
		const AGE::Link &getLink() const;
		AGE::Link &getLink();
		ComponentBase *getComponent(ComponentType id);

		template <typename T, typename... Args>
		T *addComponent(Args &&...args)
		{
			auto id = Component<T>::getTypeId();
			if (haveComponent(id))
				return static_cast<T*>(getComponent(id));
			if (components.size() <= id)
				components.resize(id + 1, nullptr);
			T *ptr = scene->createComponent<T>(entity, args...);
			components[id] = ptr;
			scene->informFiltersComponentAddition(id, *this);
			ptr->reset(scene);
			ptr->init(scene, args...);
			return ptr;
		}
		void removeComponent(ComponentType id);
		bool haveComponent(ComponentType id) const;
	private:
		Entity entity;
		AGE::Link link;
		std::vector<ComponentBase*> components;
		AScene *scene;
	public:
		friend AScene;
		friend EntityFilter;
		friend ComponentManager;
	};
}