#pragma once

#include "Room.hpp"

struct PhysicsRoom : public Room
{
	PhysicsRoom(std::weak_ptr<AScene> &&scene);
	virtual ~PhysicsRoom();
	virtual bool init();
	Entity hotZonePhysicsProjection;
	std::vector<Entity> cubes;
	std::vector<Entity> lights;
	Entity clickToShoot;
protected:
	virtual bool _enable();
	virtual bool _disable();
};