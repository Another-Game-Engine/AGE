#pragma once

#include "Component.hh"
#include <Sprite/SpriteManager.hh>

namespace Component
{
	struct Sprite : public Component::ComponentBase<Sprite>
	{
		Sprite()
		: Component::ComponentBase<Sprite>()
		, animation(nullptr)
		, delay(1.0f / 3.0f)
		, timeCounter(0.0f)
		, index(0)
		{}

		virtual ~Sprite(void)
		{}

		Sprite(Sprite const &o)
		{
			animation = o.animation;
			delay = o.delay;
			timeCounter = o.timeCounter;
			index = o.index;
		}

		Sprite &operator=(Sprite const &o)
		{
			animation = o.animation;
			delay = o.delay;
			timeCounter = o.timeCounter;
			index = o.index;
			return *this;
		}


		void init(std::shared_ptr<SpriteAnimation> _animation)
		{
			animation = _animation;
			index = 0;
		}
		virtual void reset()
		{
			index = 0;
			timeCounter = 0.0f;
			animation = nullptr;
		}

		void update(float time)
		{
			timeCounter += time;
			if (timeCounter >= delay)
			{
				index = (index + static_cast<std::uint32_t>(timeCounter / delay)) % animation->getStepNumber();
				timeCounter = 0;
			}
		}

		//////
		////
		// Serialization

		template <typename Archive>
		void serialize(Archive &ar)
		{
			auto TODO = 0;
			ar(CEREAL_NVP(TODO));
		}

		// !Serialization
		////
		//////

		std::shared_ptr<SpriteAnimation> animation;
		float delay;
		float timeCounter;
		std::uint32_t index;
	private:
	};

}