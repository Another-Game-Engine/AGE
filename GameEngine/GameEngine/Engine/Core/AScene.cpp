
#include "AScene.hh"
#include <Core/Engine.hh>
#include <Entities/EntityManager.h>

AScene::AScene(Engine &engine) :
	_engine(engine)
{
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