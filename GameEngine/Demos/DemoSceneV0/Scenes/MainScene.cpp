#include "MainScene.hh"

//systems
#include <SharedSystems/FPSSystem.hh>

// SDL
#include <Context/SdlContext.hh>

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
	if (getInstance<Input>()->getInput(SDLK_ESCAPE) ||
		getInstance<Input>()->getInput(SDL_QUIT))
	{
		//SERIALIZATION
		{
			std::ofstream s("BulletScene.scenesave", std::ios_base::binary);
			save<cereal::BinaryOutputArchive>(s);
			s.close();
		}
		return false;
	}
	return true;
}
