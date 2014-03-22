#pragma once

#include <Systems/System.h>
#include <Entities/EntityData.hh>
#include <Components/HotZone.hpp>
#include <Components/Collision.hpp>
#include <MyTags.hpp>

class HotZoneSystem : public System
{
public:
	HotZoneSystem(std::weak_ptr<AScene> scene)
		: System(scene)
		, _zones(scene)
		, _lastZone(Entity())
	{}

	virtual ~HotZoneSystem()
	{}

private:
	EntityFilter _zones;
	Entity _lastZone;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		for (auto e : _zones.getCollection())
		{
			auto collision = e->getComponent<Component::Collision>();
			auto isCharacter = false;
			for (auto c : collision->getCollisions())
			{
				if (!c->isTagged(MyTags::HEROS_TAG))
					continue;
				isCharacter = true;
			}
			if (!isCharacter)
				return;
			if (!_lastZone.get())
			{
				_lastZone = e;
				return;
			}
			if (_lastZone == e)
				return;
			auto prev = _lastZone->getComponent<Component::HotZone>();
			auto curr = e->getComponent<Component::HotZone>();
			if (prev->sharedName == curr->sharedName)
			{
				if (prev->room->isEnable())
					prev->room->disable();
				else
					prev->room->enable();
				if (curr->room->isEnable())
					curr->room->disable();
				else
					curr->room->enable();
				_lastZone = e;
			}
		}
	}

	virtual void initialize()
	{
		_zones.requireComponent<Component::HotZone>();
		_zones.requireComponent<Component::Collision>();
	}
};