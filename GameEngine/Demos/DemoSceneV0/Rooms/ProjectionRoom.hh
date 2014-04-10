#pragma once

#include "Room.hpp"

struct ProjectionRoom : public Room
{
	ProjectionRoom(std::weak_ptr<AScene> &&scene);
	virtual ~ProjectionRoom();
	virtual bool init();
	Entity hotZoneEntranceProjection;
	Entity hotZoneProjectionPhysics;
	Entity hotZoneProjectionCircle;
	Entity spotLight;
	std::vector<Entity> pointLights;
	Entity boringText;
	Entity slides;
	Entity powerpoint;
	std::vector<Entity> trolls;
	std::vector<Entity> chairs;
protected:
	virtual bool _enable();
	virtual bool _disable();
};