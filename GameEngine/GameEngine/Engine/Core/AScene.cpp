
#include <Core/Engine.hh>
#include <Utils/PubSub.hpp>
#include <limits>
#include <Core/AScene.hh>
#include <Systems/System.h>
#include <Core/EntityFilter.hpp>

AScene::AScene(std::weak_ptr<Engine> engine) :
DependenciesInjector(engine)
, _entityNumber(0)
{
}

AScene::~AScene()
{
	_systems.clear();
	for (auto &e : _pool)
		e.reset();
	_pool.clear();
}

void 							AScene::update(double time)
{
	for (auto &e : _systems)
	{
		if (e.second->isActivated())
			e.second->update(time);
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
		_pool.push_back(std::move(EntityData(std::static_pointer_cast<AScene>(shared_from_this()))));
		_pool.back().setHandle(Entity(_pool.size() - 1, this));
		_free.push(_pool.size() - 1);
	}
	std::size_t index = _free.front();
	_free.pop();
	_pool[index].addFlags(EntityData::ACTIVE);
	return _pool[index].getHandle();
}

void AScene::destroy(const Entity &h)
{
	auto e = get(h);
	if (!e)
		return;
	--_entityNumber;
	e->reset();
	e->removeFlags(EntityData::ACTIVE);
	++(e->getHandle()._version);
	_free.push(h.getId());
}

EntityData *AScene::get(const Entity &h)
{
	if (h.getId() >= _pool.size())
		return nullptr;
	auto res = &_pool[h.getId()];
	if (res->getHandle() != h)
		return nullptr;
	return &_pool[h.getId()];
}

void AScene::filterSubscribe(unsigned short id, EntityFilter* filter)
{
	if (_filters.find(id) != std::end(_filters))
		return;
	_filters[id].push_back(filter);
}

void AScene::filterUnsubscribe(unsigned short id, EntityFilter* filter)
{
	if (_filters.find(id) == std::end(_filters))
		return;
	_filters[id].remove(filter);
}

void AScene::informFilters(bool added, unsigned short id, Entity &&entity)
{
	if (_filters.find(id) == std::end(_filters))
		return;
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
