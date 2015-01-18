#pragma once

#include <Systems/System.h>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>

namespace AGE
{
	class CollisionCleaner : public System
	{
	public:
		CollisionCleaner(std::weak_ptr<AScene> &&scene)
			: System(std::move(scene))
			, _filter(std::move(scene))
		{
			_name = "collision_cleaner_system";
		}
		virtual ~CollisionCleaner(){}
	private:
		EntityFilter _filter;

		virtual void updateBegin(double time)
		{
		}

		virtual void updateEnd(double time)
		{}

		virtual void mainUpdate(double time)
		{
			_scene.lock()->clearComponentsType<Component::Collision>();
		}

		virtual bool initialize()
		{
			_filter.requireComponent<Component::Collision>();
			return true;
		}
	};
}