#include <System/System.h>
#include <Core/AScene.hh>

namespace AGE
{
	SystemType SystemBase::_typeCounter = 0;

	SystemBase::SystemBase(AScene *scene, const SystemType typeId) :
		_scene(scene)
		, _activated(false)
		, _typeId(typeId)
	{
		AGE_ASSERT(_typeId != std::size_t(-1));
	}

	SystemBase::~SystemBase()
	{
	}


	void SystemBase::update(float time)
	{
		updateBegin(time);
		mainUpdate(time);
		updateEnd(time);
	}

	bool SystemBase::init()
	{
		if (!initialize())
			return false;
		return setActivation(true);
	}

	bool SystemBase::setActivation(bool tof)
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

	bool SystemBase::isActivated() const
	{
		return _activated;
	}
	
	void SystemBase::finalize(void)
	{
	}
	
	void SystemBase::updateBegin(float time) 
	{
	}

	void SystemBase::updateEnd(float time)
	{
	}

	void SystemBase::mainUpdate(float time)
	{
	}

	bool SystemBase::initialize()
	{
		return true;
	}
	
	bool SystemBase::activate()
	{
		return true;
	}

	bool SystemBase::deactivate()
	{
		return true;
	}

}