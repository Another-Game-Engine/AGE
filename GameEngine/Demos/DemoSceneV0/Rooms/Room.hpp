#pragma once

#include <memory>
#include <Core/AScene.hh>

struct Room
{
	Room(std::weak_ptr<AScene> scene)
		: _scene(scene)
	{
	}

	virtual ~Room()
	{}

	virtual bool init() = 0;

	inline bool isEnable() const { return _enabled; }
	inline bool enable()
	{
		if (_enabled)
			return false;
		_enabled = true;
		return _enable();
	}

	inline bool disable()
	{
		if (!_enabled)
			return false;
		_enabled = false;
		return _disable();
	}
protected:
	std::weak_ptr<AScene> _scene;
	virtual bool _enable() = 0;
	virtual bool _disable() = 0;
private:
	bool _enabled = false;
};