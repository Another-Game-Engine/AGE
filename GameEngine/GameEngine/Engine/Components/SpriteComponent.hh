#pragma once

#include "Component.hh"
#include <Sprite/SpriteManager.hh>
#include <OpenGL/Shader.hh>

namespace Component
{
	struct Sprite : public Component::ComponentBase<Sprite>
	{
		Sprite()
		: Component::ComponentBase<Sprite>()
		, animation(nullptr)
		, shader(nullptr)
		, delay(1.0f / 3.0f)
		, timeCounter(0.0f)
		, index(0)
		{}

		virtual ~Sprite(void)
		{}

		void init(std::shared_ptr<SpriteAnimation> _animation, std::shared_ptr<OpenGLTools::Shader> _shader)
		{
			animation = _animation;
			shader = _shader;
			index = 0;
		}
		virtual void reset()
		{
			index = 0;
			timeCounter = 0.0f;
			animation = nullptr;
			shader = nullptr;
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
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new Sprite();
			res->setEntity(e);
			ar(*res);
			return res;
		}

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
		std::shared_ptr<OpenGLTools::Shader> shader;
		float delay;
		float timeCounter;
		std::uint32_t index;
	private:
		Sprite(Sprite const &);
		Sprite &operator=(Sprite const &);
	};

}