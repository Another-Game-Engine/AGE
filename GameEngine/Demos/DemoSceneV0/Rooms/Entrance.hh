#pragma once

#include "Room.hpp"

struct Entrance : public Room
{
	Entrance(std::weak_ptr<AScene> scene);
	virtual ~Entrance();
	virtual bool init();
	Entity room;
	Entity trololo;
	Entity pong;
	Entity hotZone;
	std::vector<Entity> lights;
protected:
	virtual bool _enable();
	virtual bool _disable();
};