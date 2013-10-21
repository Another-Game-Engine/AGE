
#include <stdlib.h>
#include "Core\Engine.hh"

int			main(int ac, char **av)
{
	if (GameEngine::instance()->start() == false)
		return (EXIT_FAILURE);
	while (GameEngine::instance()->update())
		GameEngine::instance()->draw();
	GameEngine::instance()->stop();
	return (EXIT_SUCCESS);
}