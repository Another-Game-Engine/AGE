
#include <Core/Engine.hh>
#include <Utils/PubSub.hpp>
#include <limits>
#include <Core/AScene.hh>
#include <Systems/System.h>
#include <Core/EntityFilter.hpp>

AScene::AScene(std::weak_ptr<Engine> &&engine) :
DependenciesInjector(std::move(engine))
, _entityNumber(0)
{
}

AScene::~AScene()
{
	_systems.clear();
}

void 							AScene::update(double time)
{
	static double reorderingTime = 0.0f;
	for (auto &e : _systems)
	{
		if (e.second->isActivated())
			e.second->update(time);
	}
	reorderingTime += time;
	if (reorderingTime > 0.4)
	{
		reorganizeComponents();
		reorderingTime = 0;
	}
}

bool                           AScene::start()
{
	setInstance<PubSub::Manager>();
	return userStart();
}

Entity &AScene::createEntity()
{
	++_entityNumber;
	if (_free.empty())
	{
		auto &e = _pool[_entityNumber];
		e.init(_entityNumber);
		assert(++_entityNumber != UINT16_MAX);
		return e;
	}
	else
	{
		auto id = _free.back();
		_free.pop();
		return _pool[id];
	}
}

void AScene::destroy(Entity &e)
{
	e.reset();
	_pool[e.id].reset();
	_free.push(e.id);
}

glm::mat4 &AScene::getLocalTransform(const Entity &e)
{
	//TODO
	return glm::mat4(1);
}

glm::mat4 &AScene::getGlobalTransform(const Entity &e)
{
	//TODO
	return glm::mat4(1);
}

void AScene::filterSubscribe(unsigned short id, EntityFilter* filter)
{
	auto findIter = std::find(_filters[id].begin(), _filters[id].end(), filter);
	if (findIter == std::end(_filters[id]))
		_filters[id].push_back(filter);
}

void AScene::filterUnsubscribe(unsigned short id, EntityFilter* filter)
{
	_filters[id].remove(filter);
}

void AScene::informFilters(bool added, std::uint8_t id, Entity &&entity)
{
	if (added)
	{
		for (auto &&f : _filters[id])
		{
			f->componentAdded(std::move(entity), id);
		}
	}
	else
	{
		for (auto &&f : _filters[id])
		{
			f->componentRemoved(std::move(entity), id);
		}
	}
}
