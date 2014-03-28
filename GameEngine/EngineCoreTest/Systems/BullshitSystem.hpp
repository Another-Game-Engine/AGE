#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Systems/System.h"

#include <Entities/EntityData.hh>

namespace Component
{
	struct Bullshit : public Component::ComponentBase<Bullshit>
	{
		Bullshit()
		{

		}

		virtual ~Bullshit(void)
		{

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
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new Bullshit();
			res->setEntity(e);
			ar(*res);
			return res;
		}

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
		Bullshit(Bullshit const &);
		Bullshit &operator=(Bullshit const &);
	};
}

class BullshitSystem : public System
{
public:
	BullshitSystem(std::weak_ptr<AScene> scene)
		: System(scene)
		, _filter(scene)
	{
		_name = "rotation_force_system";
	}
	virtual ~BullshitSystem(){}
private:
	EntityFilter _filter;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		float t = static_cast<float>(time);
		for (auto e : _filter.getCollection())
		{
			e->getComponent<Component::Bullshit>()->_t += t;
			//if (e->getComponent<Component::Bullshit>()->_t > 0.55f)
			//	e->removeComponent<Component::Bullshit>();
		}
	}

	virtual bool initialize()
	{
		_filter.requireComponent<Component::Bullshit>();
		return true;
	}
};