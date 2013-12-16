
#include "AScene.hh"
#include <Core/Engine.hh>
#include <Entities/EntityManager.h>

AScene::AScene(Engine &engine) :
	_camera(NULL),
	_engine(engine)
{
}

AScene::~AScene()
{
	if (_camera)
		delete _camera;
	_systems.clear();
}


void 							AScene::setCamera(ACamera *camera)
{
	_camera = camera;
}

ACamera 						*AScene::getCamera() const
{
	return (_camera);
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