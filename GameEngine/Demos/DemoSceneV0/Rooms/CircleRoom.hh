#pragma once

#include "Room.hpp"

struct CircleRoom : public Room
{
	CircleRoom(std::weak_ptr<AScene> scene);
	virtual ~CircleRoom();
	virtual bool init();
	Entity hotZone;
protected:
	virtual bool _enable();
	virtual bool _disable();
};