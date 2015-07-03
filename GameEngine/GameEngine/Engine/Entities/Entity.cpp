#include "Entity.hh"
#include <Core/AScene.hh>
#include <Components/ComponentRegistrationManager.hpp>

namespace AGE
{
	EntityData::EntityData(AScene *_scene)
		: link(this, _scene)
		, scene(_scene)
		, outOfContext(false)
	{
	}

	const Entity &EntityData::getEntity() const { return entity; }
	Entity &EntityData::getEntity() { return entity; }
	const AGE::Link &EntityData::getLink() const { return link; }
	AGE::Link &EntityData::getLink() { return link; }
	AScene *EntityData::getScene() { return scene; }

	ComponentBase *EntityData::getComponent(ComponentType id)
	{
		if (!haveComponent(id))
			return nullptr;
		return components[id];
	}

	void EntityData::addComponentPtr(ComponentBase *cpt)
	{
		auto id = cpt->getType();
		if (haveComponent(id))
		{
			// if already have component of this type, we remove the old one
			removeComponent(id);
		}
		if (components.size() <= id)
		{
			components.resize(id + 1, nullptr);
		}
		components[id] = cpt;
		if (!outOfContext)
		{
			scene->informFiltersComponentAddition(id, *this);
		}
	}

	void EntityData::copyComponent(ComponentBase *cpt)
	{
		auto id = cpt->getType();
		auto newCpt = ComponentRegistrationManager::getInstance().copyComponent(cpt, scene);
		newCpt->entity = getEntity();
		newCpt->reset();
		newCpt->copyFrom(cpt);

		if (haveComponent(id))
		{
			// if already have component of this type, we remove the old one
			removeComponent(id);
		}

		if (components.size() <= id)
		{
			components.resize(id + 1, nullptr);
		}
		components[id] = newCpt;
		if (!outOfContext)
		{
			scene->informFiltersComponentAddition(id, *this);
		}
	}

	void EntityData::removeComponent(ComponentType id)
	{
		if (!haveComponent(id))
			return;
		components[id]->reset();
		scene->deleteComponent(components[id]);
		components[id] = nullptr;
		if (!outOfContext)
		{
			scene->informFiltersComponentDeletion(id, *this);
		}
	}

	bool EntityData::haveComponent(ComponentType id) const
	{
		return !(components.size() <= id || components[id] == nullptr);
	}

	/////////////////////////////////////

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

	const Entity *Entity::getPtr() const
	{
		return &ptr->getEntity();
	}

	AGE::Link *Entity::getLinkPtr()
	{
		return &ptr->getLink();
	}

	const AGE::Link *Entity::getLinkPtr() const
	{
		return &ptr->getLink();
	}
}