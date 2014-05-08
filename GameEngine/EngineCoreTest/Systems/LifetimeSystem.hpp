#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Systems/System.h"

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
			_t = static_cast<float>(rand() % 100) / 10.0f;
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

		Lifetime &operator=(const Lifetime &o)
		{
			_t = o._t;
			return *this;
		}

	private:
		//Lifetime(Lifetime const &);
		//Lifetime &operator=(Lifetime const &);
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
		auto scene = this->_scene.lock();
		EntityFilter::Lock lock(_filter);
		auto &collection = _filter.getCollection();
		for (Entity e : _filter.getCollection())
		{
			auto lifetime = scene->getComponent<Component::Lifetime>(e);
			lifetime->_t -= t;
			if (scene->getComponent<Component::Lifetime>(e)->_t <= 0.0f)
				scene->destroy(e);
				//scene->removeComponent<Component::Lifetime>(const_cast<Entity&>(e));
		}
	}

	virtual bool initialize()
	{
		_filter.requireComponent<Component::Lifetime>();
		return true;
	}
};