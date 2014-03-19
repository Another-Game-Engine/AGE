#pragma once

#include <Core/AScene.hh>

struct Room
{
	Room(std::weak_ptr<AScene> scene)
		: _scene(scene)
	{
	}

	virtual ~Room()
	{}

	inline bool isEnable() const { return _enabled; }
	inline bool enable()
	{
		if (_enabled)
			return false;
		return _enable();
	}

	inline bool disable()
	{
		if (!_enabled)
			return false;
		return _disable();
	}
protected:
	std::weak_ptr<AScene> _scene;
	virtual bool _enable() = 0;
	virtual bool _disable() = 0;
private:
	bool _enabled = false;
};

struct Entrance : public Room
{
	Entrance(std::weak_ptr<AScene> scene)
		: Room(scene)
	{
		room = scene.lock()->createEntity();
	}

	virtual ~Entrance()
	{
		_scene.lock()->destroy(room);
	}

	Entity room;
protected:
	virtual bool _enable()
	{
		return true;
	}

	virtual bool _disable()
	{
		return true;
	}
};

struct ProjectionRoom : public Room
{
	ProjectionRoom(std::weak_ptr<AScene> scene)
		: Room(scene)
	{
		room = scene.lock()->createEntity();
	}

	virtual ~ProjectionRoom()
	{
		_scene.lock()->destroy(room);
	}

	Entity room;
protected:
	virtual bool _enable()
	{
		return true;
	}

	virtual bool _disable()
	{
		return true;
	}
};

class MainScene : public AScene	
{
public:
	MainScene(std::weak_ptr<Engine> engine);
	virtual ~MainScene(void);
	virtual bool 			userStart();
	virtual bool 			userUpdate(double time);
private:
	Entity _heros;
	std::unique_ptr<Entrance> _entrance;
	std::unique_ptr<ProjectionRoom> _projectionRoom;
};