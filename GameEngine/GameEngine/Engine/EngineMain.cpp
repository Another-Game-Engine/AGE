
#include <stdlib.h>
#include "Core/Engine.hh"
#include "Demo/DemoScene.hh"

int			main(int ac, char **av)
{
	GameEngine::instance()->setContext(new SdlContext);
	GameEngine::instance()->addScene(new DemoScene, "demo");
	GameEngine::instance()->bindScene("demo");
	if (GameEngine::instance()->start() == false)
		return (EXIT_FAILURE);
	while (GameEngine::instance()->update())
		GameEngine::instance()->draw();
	GameEngine::instance()->stop();
	return (EXIT_SUCCESS);
}