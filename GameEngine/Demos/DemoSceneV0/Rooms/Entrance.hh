#pragma once

#include "Room.hpp"

struct Entrance : public Room
{
	Entrance(std::weak_ptr<AScene> scene);
	virtual ~Entrance();
	virtual bool init();
	Entity room;
protected:
	virtual bool _enable();
	virtual bool _disable();
};