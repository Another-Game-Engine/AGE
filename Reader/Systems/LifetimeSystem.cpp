#include "LifetimeSystem.hpp"
#include <ComponentsCore/Lifetime.hpp>
#include <Entity/Entity.hh>
#include <Core/AScene.hh>

namespace AGE
{
	LifetimeSystem::LifetimeSystem(AScene *scene)
		: System(std::move(scene))
		, _filter(std::move(scene))
	{
		_name = "lifetime_system";
	}

	LifetimeSystem::~LifetimeSystem()
	{}

	void LifetimeSystem::updateBegin(float time)
	{}

	void LifetimeSystem::updateEnd(float time)
	{}

	void LifetimeSystem::mainUpdate(float time)
	{
		auto scene = _scene;
		EntityFilter::Lock lock(_filter);
		auto &collection = _filter.getCollection();
		for (auto &e : collection)
		{
			e->getComponent<Lifetime>()->_t -= time;
			if (e->getComponent<Lifetime>()->_t <= 0.0f)
				scene->destroy(e);
		}
	}

	bool LifetimeSystem::initialize()
	{
		_filter.requireComponent<Lifetime>();
		return true;
	}
}