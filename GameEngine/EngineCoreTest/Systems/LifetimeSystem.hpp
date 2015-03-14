#pragma once

#include "Systems/System.h"
#include <Components/Component.hh>
#include <Entities/Entity.hh>

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

		void init(float t = 1.0f)
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

	class LifetimeSystem : public AGE::System
	{
	public:
		LifetimeSystem(AScene *scene)
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
			auto scene = _scene;
			EntityFilter::Lock lock(_filter);
			auto &collection = _filter.getCollection();
			for (auto e : collection)
			{
				e.getComponent<Lifetime>()->_t -= t;
				if (e.getComponent<Lifetime>()->_t <= 0.0f)
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

