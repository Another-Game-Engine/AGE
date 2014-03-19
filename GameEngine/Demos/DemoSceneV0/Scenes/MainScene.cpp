#include "MainScene.hh"

//systems
#include <SharedSystems/FPSSystem.hh>

MainScene::MainScene(std::weak_ptr<Engine> engine)
: AScene(engine)
{}

MainScene::~MainScene(void)
{}


bool 			MainScene::userStart()
{
	// add systems
	addSystem<FPSSystem>(0);

	return true;
}


bool 			MainScene::userUpdate(double time)
{
	return true;
}
