#include <Systems/System.h>
#include <Core/AScene.hh>

namespace AGE
{
	System::System(AScene *scene) :
		_scene(scene)
		, _activated(false)
	{}

	System::~System()
	{
	}


	void System::update(float time)
	{
		updateBegin(time);
		mainUpdate(time);
		updateEnd(time);
	}

	bool System::init()
	{
		if (!initialize())
			return false;
		return setActivation(true);
	}

	bool System::setActivation(bool tof)
	{
		auto res = false;
		if (tof && _activated)
			return true;
		else if (tof && !_activated)
		{
			res = activate();
			_activated = res == true ? res : false;
		}
		else if (!tof && !_activated)
			return true;
		else if (!tof && _activated)
		{
			res = deactivate();
			_activated = false;
		}
		return res;
	}

	bool System::isActivated() const
	{
		return _activated;
	}
	
	void System::finalize(void)
	{
	}
	
	void System::updateBegin(float time) 
	{
	}

	void System::updateEnd(float time)
	{
	}

	void System::mainUpdate(float time)
	{
	}

	bool System::initialize()
	{
		return true;
	}
	
	bool System::activate()
	{
		return true;
	}

	bool System::deactivate()
	{
		return true;
	}

}