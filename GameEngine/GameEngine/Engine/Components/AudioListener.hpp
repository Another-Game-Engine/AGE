#pragma once

#include <Components/Component.hh>
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>
#include <Audio/Audio.hh>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>

namespace Component
{
	struct AudioListener : public Component::ComponentBase<AudioListener>
	{
		AudioListener();
		virtual ~AudioListener(void);
		void init();
		virtual void reset();

		//////
		////
		// Serialization

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