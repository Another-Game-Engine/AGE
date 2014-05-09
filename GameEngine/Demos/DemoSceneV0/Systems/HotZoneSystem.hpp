#pragma once

#include <Systems/System.h>
#include <Components/HotZone.hpp>
#include <Components/Collision.hpp>
#include <MyTags.hpp>
#include <Rooms/Room.hpp>

class HotZoneSystem : public System
{
public:
	HotZoneSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _zones(std::move(scene))
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
		auto scene = _scene.lock();
		for (auto e : _zones.getCollection())
		{
			auto collision = scene->getComponent<Component::Collision>(e);
			auto isCharacter = false;
			for (auto c : collision->getCollisions())
			{
				if (scene->isTagged(c, MyTags::HEROS_TAG))
					continue;
				isCharacter = true;
			}
			if (!isCharacter)
				return;
			if (_lastZone == e)
				return;
			auto prev = scene->getComponent<Component::HotZone>(_lastZone);
			auto curr = scene->getComponent<Component::HotZone>(e);
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
			else
			{
				_lastZone = e;
			}
		}
	}

	virtual bool initialize()
	{
		_zones.requireComponent<Component::HotZone>();
		_zones.requireComponent<Component::Collision>();
		return true;
	}
};