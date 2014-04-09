#include <Components/SpaceshipController.hpp>

namespace Component
{

	SpaceshipController::SpaceshipController() :
		ComponentBase<SpaceshipController>(),
		dist(0.0f)
	{}

	SpaceshipController::~SpaceshipController(void)
	{}

	void SpaceshipController::init(const Entity &e, float distance)
	{
		spaceShip = e;
		dist = distance;
	}

	void SpaceshipController::reset()
	{}

}
