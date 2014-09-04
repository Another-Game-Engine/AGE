#include <Systems/LifetimeSystem.hh>
#include <Components/lifetimeComponent.hh>

LifetimeSystem::LifetimeSystem(std::weak_ptr<AScene> &&scene)
	: System(std::move(scene))
	, _filter(std::move(scene))
{
	_name = "lifetime_system";
}

LifetimeSystem::~LifetimeSystem()
{

}

void LifetimeSystem::updateBegin(double time)
{
}

void LifetimeSystem::updateEnd(double time)
{
}

void LifetimeSystem::mainUpdate(double time)
{
	float t = static_cast<float>(time);
	auto scene = _scene.lock();
	EntityFilter::Lock lock(_filter);
	for (auto &&e : _filter.getCollection())
	{
		scene->getComponent<Component::Lifetime>(e)->_t -= t;
		if (scene->getComponent<Component::Lifetime>(e)->_t <= 0.0f)
			scene->destroy(e);
	}
}

bool LifetimeSystem::initialize()
{
	_filter.requireComponent<Component::Lifetime>();
	return true;
}