#pragma once

#include "Room.hpp"
#include <map>

struct EngineRoom : public Room
{
	EngineRoom(std::weak_ptr<AScene> scene);
	virtual ~EngineRoom();
	virtual bool init();
	Entity hotZoneEngineCircle;
	Entity hotZoneEngineLast;
	Entity hotZoneEngineSponza;
	Entity hotZoneEngineAsteroid;
	Entity hotZoneEngineInception;


	std::map<std::string, Entity> map;
	//PubSub fboAsteroidReceiver;
	//GLuint fboAsteroidId;
	//PubSub fboSponzaReceiver;
	//GLuint fboSponzaId;
protected:
	virtual bool _enable();
	virtual bool _disable();
};