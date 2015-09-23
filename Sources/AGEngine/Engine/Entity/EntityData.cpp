#include "EntityData.hh"
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
		SCOPE_profile_cpu_function("Entity");

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
		SCOPE_profile_cpu_function("Entity");

		auto id = cpt->getType();
		auto newCpt = ComponentRegistrationManager::getInstance().copyComponent(cpt, scene);
		if (!newCpt)
		{
			return;
		}
		newCpt->entity = getEntity();
		newCpt->reset();
		newCpt->copyFrom(cpt);

		if (haveComponent(id))
		{
			// if already have component of this type, we remove the old one
			//removeComponent(id);
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
		SCOPE_profile_cpu_function("Entity");

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

	const std::vector<ComponentBase*> &EntityData::getComponentList() const
	{
		return components;
	}

}