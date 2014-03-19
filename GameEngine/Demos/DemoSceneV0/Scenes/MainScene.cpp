#include "MainScene.hh"

MainScene::MainScene(std::weak_ptr<Engine> engine)
: AScene(engine)
{}

MainScene::~MainScene(void)
{}


bool 			MainScene::userStart()
{
	return true;
}


bool 			MainScene::userUpdate(double time)
{
	return true;
}
