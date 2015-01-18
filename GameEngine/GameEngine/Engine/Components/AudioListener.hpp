#pragma once

#include <Components/Component.hh>
#include <glm/fwd.hpp>
#include <Utils/GlmSerialization.hpp>
#include <Audio/Audio.hh>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>

namespace AGE
{
	namespace Component
	{
		struct AudioListener : public Component::ComponentBase < AudioListener >
		{
			AudioListener();
			virtual ~AudioListener(void);
			void init(AScene *);
			virtual void reset(AScene *);
			AudioListener(AudioListener const &){}
			AudioListener &operator=(AudioListener const &){ return *this; }
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
		};
	}
}