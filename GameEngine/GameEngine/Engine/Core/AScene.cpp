
#include <Core/Engine.hh>
#include <Utils/PubSub.hpp>
#include <limits>
#include <Core/AScene.hh>
#include <Systems/System.h>

AScene::AScene(Engine &engine) :
	_engine(engine)
{
	setInstance<PubSub::Manager>();
}

AScene::~AScene()
{
	_systems.clear();
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

Handle &AScene::createEntity()
{
	if (_free.empty())
	{
		_pool.push_back(Entity(this));
		_pool.back().setHandle(Handle(_pool.size() - 1, this));
		_free.push(_pool.size() - 1);
	}
	unsigned int index = _free.back();
	_free.pop();
	return _pool[index].getHandle();
}

void AScene::destroy(const Handle &h)
{
	_free.push(h.getId());
	h.get()->reset();
}

Entity *AScene::get(const Handle &h)
{
	if (h.getId() >= _pool.size())
		return nullptr;
	return &_pool[h.getId()];
}