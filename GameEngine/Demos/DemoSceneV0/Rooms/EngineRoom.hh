#pragma once

#include "Room.hpp"
#include <map>
class DemoCatSystem;


struct EngineRoom : public Room
{
	EngineRoom(std::weak_ptr<AScene> scene);
	virtual ~EngineRoom();
	virtual bool init();
	Entity hotZoneEngineCircle;
	Entity hotZoneEngineLast;
	Entity hotZoneEngineSponza;
	Entity hotZoneEngineAsteroid;

	std::map<std::string, Entity> map;
protected:
	virtual bool _enable();
	virtual bool _disable();
};