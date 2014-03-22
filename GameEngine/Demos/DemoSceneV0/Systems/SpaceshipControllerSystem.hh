#pragma once

#include <Systems\System.h>
#include <Core/EntityFilter.hpp>

class SpaceshipControllerSystem : public System
{
public:
	SpaceshipControllerSystem(std::weak_ptr<AScene> scene);
	virtual ~SpaceshipControllerSystem();

	virtual void updateBegin(double time) { }
	virtual void updateEnd(double time) { }
	virtual void mainUpdate(double time);
	virtual void initialize();

private:
	EntityFilter						_spaceshipsFilter;
};

