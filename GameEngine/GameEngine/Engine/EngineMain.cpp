
#include <stdlib.h>
#include "Core/Engine.hh"
#include "Demo/DemoScene.hh"

int			main(int ac, char **av)
{
	Engine	&e = *GameEngine::instance();

	e.setContext(new SdlContext);
	e.addScene(new DemoScene, "demo");
	e.bindScene("demo");
	if (e.start() == false)
		return (EXIT_FAILURE);
	while (e.update())
		e.draw();
	e.stop();
	return (EXIT_SUCCESS);
}