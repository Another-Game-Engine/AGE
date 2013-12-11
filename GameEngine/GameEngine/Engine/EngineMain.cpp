// for leak detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#include <stdlib.h>
#include "Core/Engine.hh"
#include "Demo/DemoScene.hh"
#include "Utils/PubSub.hpp"
#include "Context/SdlContext.hh"

int			main(int ac, char **av)
{
	Engine	e;

	e.setInstance<SdlContext, IRenderContext>();
	if (e.init() == false)
		return (EXIT_FAILURE);
	e.addScene(new DemoScene(e), "demo");
	e.bindScene("demo");
	if (e.start() == false)
		return (EXIT_FAILURE);
	while (e.update())
		e.draw();
	e.stop();
	return (EXIT_SUCCESS);
}