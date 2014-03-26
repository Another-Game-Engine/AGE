#pragma once

#include <Systems\System.h>
#include <Core/EntityFilter.hpp>

class AsteroidSystem : public System
{
public:
	AsteroidSystem(std::weak_ptr<AScene> scene);
	virtual ~AsteroidSystem();

	virtual void updateBegin(double time) { }
	virtual void updateEnd(double time) { }
	virtual void mainUpdate(double time);
	virtual bool initialize();

private:
	EntityFilter						_asteroidFilter;
	EntityFilter						_bulletFilter;
};

