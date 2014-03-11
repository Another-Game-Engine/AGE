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
		, fps(60)
		, index(0)
		{}

		virtual ~Sprite(void)
		{}

		void init(std::shared_ptr<SpriteAnimation> _animation, OpenGLTools::Shader *_shader)
		{
			animation = _animation;
			shader = _shader;
			index = 0;
		}
		virtual void reset()
		{
			animation = nullptr;
			shader = nullptr;
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
		OpenGLTools::Shader *shader;
		float fps;
		std::size_t index;
	private:
		Sprite(Sprite const &);
		Sprite &operator=(Sprite const &);
	};

}