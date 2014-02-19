
#include <Core/Engine.hh>
#include <Utils/PubSub.hpp>
#include <limits>
#include <Core/AScene.hh>
#include <Systems/System.h>

AScene::AScene(Engine &engine) :
DependenciesInjector(&engine),
_engine(engine)
{
	setInstance<PubSub::Manager>();
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
		e.second->update(time);
	}
	//_root->computeGlobalTransform(glm::mat4(1));
	//recomputePositions(_engine.getInstance<EntityManager>().getRoot(), false);
}

Entity &AScene::createEntity()
{
	if (_free.empty())
	{
		_pool.push_back(EntityData(this));
		_pool.back().setHandle(Entity(_pool.size() - 1, this));
		_free.push(_pool.size() - 1);
	}
	unsigned int index = _free.front();
	_free.pop();
	_pool[index].addFlags(EntityData::ACTIVE);
	return _pool[index].getHandle();
}

void AScene::destroy(const Entity &h)
{
	h.get()->reset();
	h.get()->removeFlags(EntityData::ACTIVE);
	_free.push(h.getId());
}

EntityData *AScene::get(const Entity &h)
{
	if (h.getId() >= _pool.size())
		return nullptr;
	return &_pool[h.getId()];
}