#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Systems/System.h"

#include <Entities/EntityData.hh>

namespace Component
{
	struct Lifetime : public Component::ComponentBase<Lifetime>
	{
		Lifetime()
		: _t(0.0f)
		{
		}

		virtual ~Lifetime(void)
		{

		}

		void init()
		{
			_t = static_cast<float>(rand() % 10000) / 10.0f;
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
		Lifetime(Lifetime const &);
		Lifetime &operator=(Lifetime const &);
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
		EntityFilter::Lock lock(_filter);
		for (auto &&e : _filter.getCollection())
		{
			e->getComponent<Component::Lifetime>()->_t -= t;
			if (e->getComponent<Component::Lifetime>()->_t <= 0.0f)
				e->removeComponent<Component::Lifetime>();
		}
	}

	virtual bool initialize()
	{
		_filter.requireComponent<Component::Lifetime>();
		return true;
	}
};