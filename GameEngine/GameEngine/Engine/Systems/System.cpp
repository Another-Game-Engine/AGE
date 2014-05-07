#include <Systems/System.h>
#include <Core/AScene.hh>

System::System(std::weak_ptr<AScene> &&scene) :
 _scene(std::move(scene))
, _activated(false)
{}

System::~System()
{
}


void System::update(double time)
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