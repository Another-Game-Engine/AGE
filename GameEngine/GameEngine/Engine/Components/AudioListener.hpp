#pragma once

#include <Components/Component.hh>
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>
#include <Audio/Audio.hpp>

namespace Component
{
	struct AudioListener : public Component::ComponentBase<AudioListener>
	{
		AudioListener() :
			Component::ComponentBase<Component::AudioListener>()
		{}
		virtual ~AudioListener(void)
		{}

		void init()
		{
		}

		virtual void reset()
		{}

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new AudioListener();
			res->setEntity(e);
			ar(*res);
			return res;
		}

		template <typename Archive>
		void serialize(Archive &ar)
		{
		}

		// !Serialization
		////
		//////
	private:
		AudioListener(AudioListener const &);
		AudioListener &operator=(AudioListener const &);
	};
}