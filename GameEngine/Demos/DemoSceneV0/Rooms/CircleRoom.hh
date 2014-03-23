#pragma once

#include "Room.hpp"

struct CircleRoom : public Room
{
	CircleRoom(std::weak_ptr<AScene> scene);
	virtual ~CircleRoom();
	virtual bool init();
	Entity hotZoneCircleProjection;
	Entity hotZoneCircleEngine;
	Entity dragon;
	Entity rotationAxis;
	std::vector<Entity> spotLights;
protected:
	virtual bool _enable();
	virtual bool _disable();
};