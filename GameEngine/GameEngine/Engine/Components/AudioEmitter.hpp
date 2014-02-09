#pragma once

#include <Components/Component.hh>
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>
#include <Audio/Audio.hh>
#include <Audio/ChannelGroupType.hpp>
#include <Utils/MatrixConversion.hpp>

namespace Component
{
	struct AudioEmitter : public Component::ComponentBase<AudioEmitter>
	{
		struct AudioInstance
		{
			AudioInstance();
			~AudioInstance();
			void setReverb(FMOD_REVERB_PROPERTIES prop); // @TODO -> to implement
			std::shared_ptr<Audio> audio;
			FMOD::Channel *channel;
			ChannelGroupType channelGroupType;
			std::string name;
			FMOD::Reverb *reverb;
		};

		AudioEmitter();
		virtual ~AudioEmitter(void);
		void init();
		virtual void reset();
		void clearAllAudios();
		void clearAudio(const std::string &name);
		AudioInstance *getAudio(const std::string &name);
		bool hasAudio(const std::string &name);
		void setAudio(std::shared_ptr<Audio> audio, const std::string &name, ChannelGroupType groupType);
		void updatePosition();
		void play(const std::string &name, bool force);

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new AudioEmitter();
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
		std::map<std::string, AudioInstance> audios;
		AudioEmitter(AudioEmitter const &);
		AudioEmitter &operator=(AudioEmitter const &);
	};
}