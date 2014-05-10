
#include <Core/Engine.hh>
#include <Utils/PubSub.hpp>
#include <limits>
#include <Core/AScene.hh>
#include <Systems/System.h>
#include <Core/EntityFilter.hpp>
#include <Entities/EntityFlags.hh>

AScene::AScene(std::weak_ptr<Engine> &&engine) :
DependenciesInjector(std::move(engine))
, _entityNumber(0)
{
	_componentsManagers.assign(nullptr);
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


void                    AScene::informFiltersTagAddition(TAG_ID id, EntityData &&entity)
{
	for (auto &&f : _filters[id])
	{
		f->tagAdded(std::move(entity), id);
	}
}
void                    AScene::informFiltersTagDeletion(TAG_ID id, EntityData &&entity)
{
	for (auto &&f : _filters[id])
	{
		f->tagRemoved(std::move(entity), id);
	}
}

void                    AScene::informFiltersComponentAddition(COMPONENT_ID id, EntityData &&entity)
{
	for (auto &&f : _filters[id])
	{
		f->componentAdded(std::move(entity), id);
	}
}

void                    AScene::informFiltersComponentDeletion(COMPONENT_ID id, EntityData &&entity)
{
	for (auto &&f : _filters[id])
	{
		f->componentRemoved(std::move(entity), id);
	}
}

const glm::mat4 &AScene::getGlobalTransform(const Entity &e) const
{
	return _globalTransform[e.id];
}

const glm::mat4 &AScene::getLocalTransform(const Entity &e) const
{
	return _localTransform[e.id];
}

void AScene::setLocalTransform(Entity &e, const glm::mat4 &trans, bool notifyTransformation)
{
	if (notifyTransformation)
		e.setFlags() |= Flags::HasMoved;
	_localTransform[e.id] = trans;
	_globalTransform[e.id] = trans;
}
