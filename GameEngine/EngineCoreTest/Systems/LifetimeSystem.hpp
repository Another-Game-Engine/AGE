#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Systems/System.h"
#include <Components/Component.hh>

namespace Component
{
	struct Lifetime : public Component::ComponentBase<Lifetime>
	{
		Lifetime()
		{

		}

		virtual ~Lifetime(void)
		{

		}

		Lifetime(Lifetime &&o)
			: ComponentBase<Lifetime>(std::move(o))
		{
				_t = std::move(o._t);
		}

		Lifetime &operator=(Lifetime &&o)
		{
			_t = std::move(o._t);
			return *this;
		}

		void init(float t)
		{
			_t = t;
		}

		virtual void reset()
		{
			_t = 0.0f;
		}

		//////
		////
		// Serialization

		template <typename Archive>
		void serialize(Archive &ar)
		{
			ar(CEREAL_NVP(_t));
		}

		// !Serialization
		////
		//////

		float _t;
	private:
		Lifetime &operator=(Lifetime const &o);
		Lifetime(Lifetime const &o);
	};
}

class LifetimeSystem : public System
{
public:
	LifetimeSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _filter(std::move(scene))
	{
		_name = "lifetime_system";
	}
	virtual ~LifetimeSystem(){}
private:
	EntityFilter _filter;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		float t = static_cast<float>(time);
		auto scene = _scene.lock();
		EntityFilter::Lock lock(_filter);
		for (auto &&e : _filter.getCollection())
		{
			scene->getComponent<Component::Lifetime>(e)->_t -= t;
			if (scene->getComponent<Component::Lifetime>(e)->_t <= 0.0f)
				scene->destroy(e);
		}
	}

	virtual bool initialize()
	{
		_filter.requireComponent<Component::Lifetime>();
		return true;
	}
};