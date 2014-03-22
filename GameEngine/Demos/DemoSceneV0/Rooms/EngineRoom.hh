#pragma once

#include "Room.hpp"

struct EngineRoom : public Room
{
	EngineRoom(std::weak_ptr<AScene> scene);
	virtual ~EngineRoom();
	virtual bool init();
	Entity cat;
	Entity house;
	Entity hotZoneEngineCircle;
	Entity hotZoneEngineLast;
protected:
	virtual bool _enable();
	virtual bool _disable();
};