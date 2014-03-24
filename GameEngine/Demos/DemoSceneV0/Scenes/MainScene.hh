#pragma once

#include <Core/AScene.hh>
#include <Rooms/Entrance.hh>
#include <Rooms/EngineRoom.hh>
#include <Rooms/ProjectionRoom.hh>
#include <Rooms/PhysicsRoom.hh>
#include <Rooms/LastRoom.hh>
#include <Rooms/CircleRoom.hh>
#include <Rooms/SpiralRoomSponza.hpp>
#include <Rooms/SpiralRoomAsteroid.hpp>



//struct ProjectionRoom : public Room
//{
//	ProjectionRoom(std::weak_ptr<AScene> scene)
//		: Room(scene)
//	{
//		room = scene.lock()->createEntity();
//	}
//
//	virtual ~ProjectionRoom()
//	{
//		auto scene = _scene.lock();
//		if (scene)
//			scene->destroy(room);
//	}
//
//	Entity room;
//protected:
//	virtual bool _enable()
//	{
//		return true;
//	}
//
//	virtual bool _disable()
//	{
//		return true;
//	}
//};

class MainScene : public AScene	
{
public:
	MainScene(std::weak_ptr<Engine> engine);
	virtual ~MainScene(void);
	virtual bool 			userStart();
	virtual bool 			userUpdate(double time);
private:
	bool loadShaders();
	bool loadAssets();

	Entity _heros;
	std::shared_ptr<Entrance> _entrance;
	std::shared_ptr<EngineRoom> _engineRoom;
	std::shared_ptr<ProjectionRoom> _projectionRoom;
	std::shared_ptr<PhysicsRoom> _physicsRoom;
	std::shared_ptr<LastRoom> _lastRoom;
	std::shared_ptr<CircleRoom> _circleRoom;
	std::shared_ptr<SpiralRoomSponza> _sponzaRoom;
	std::shared_ptr<SpiralRoomAsteroid> _asteroidRoom;
};