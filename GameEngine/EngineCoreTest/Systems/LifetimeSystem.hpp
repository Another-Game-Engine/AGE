#pragma once

#include "Systems/System.h"
#include <Components/Component.hh>

namespace AGE
{
	struct Lifetime : public ComponentBase
	{
		Lifetime()
		{

		}

		virtual ~Lifetime(void)
		{

		}

		Lifetime(Lifetime &&o)
			: ComponentBase(std::move(o))
		{
			_t = std::move(o._t);
		}

		Lifetime &operator=(Lifetime &&o)
		{
			_t = std::move(o._t);
			return *this;
		}

		void init(AScene *, float t)
		{
			_t = t;
		}

		virtual void reset(AScene *)
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

	class LifetimeSystem : public AGE::System
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
			for (auto &e : _filter.getCollection())
			{
				scene->getComponent<Lifetime>(e)->_t -= t;
				if (scene->getComponent<Lifetime>(e)->_t <= 0.0f)
					scene->destroy(e);
			}
		}

		virtual bool initialize()
		{
			_filter.requireComponent<Lifetime>();
			return true;
		}
	};
}
