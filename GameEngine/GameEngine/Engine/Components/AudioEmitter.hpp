#pragma once

#include <Components/Component.hh>
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>
#include <Audio/Audio.hh>
#include <Audio/ChannelGroupType.hpp>
#include <Utils/MatrixConversion.hpp>
#include <Audio/AudioManager.hh>
#include <Core/AScene.hh>
#include <vector>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

namespace Component
{
	struct AudioEmitter : public Component::ComponentBase<AudioEmitter>
	{

		AudioEmitter(AudioEmitter const &o)
		{
			audios = o.audios;
		}

		AudioEmitter &operator=(AudioEmitter const &o)
		{
			audios = o.audios;
			return *this;
		}

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

		struct SerializedAudioInstance
		{
			std::string filename;
			std::string name;
			ChannelGroupType channelGroupType;
			bool isPlaying;
			float volume;
			unsigned int position;
			template <typename Archive>
			void serialize(Archive &ar)
			{
				ar(filename, name, channelGroupType, isPlaying, volume, position);
			}
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
		void save(Archive &ar) const
		{
			std::vector<SerializedAudioInstance> v;
			for (auto &e : audios)
			{
				SerializedAudioInstance a;
				a.channelGroupType = e.second.channelGroupType;
				a.filename = e.second.audio->getName();
				a.name = e.first;
				a.isPlaying = false;
				a.volume = 0.0f;
				a.position = 0;
				if (e.second.channel != nullptr)
				{
					e.second.channel->isPlaying(&a.isPlaying);
					if (a.isPlaying)
					{
						e.second.channel->getVolume(&a.volume);
						e.second.channel->getPosition(&a.position, FMOD_TIMEUNIT_MS);
					}
				}
				v.push_back(a);
			}
			ar(v);
		}


		template <typename Archive>
		void load(Archive &ar)
		{
			std::vector<SerializedAudioInstance> v;
			ar(v);
			for (auto e : v)
			{
				//@CESAR TODO
				//std::shared_ptr<Audio> a = _entity->getScene().lock()->getInstance<AudioManager>()->getAudio(e.filename);
				//if (!a)
				//	continue;
				//setAudio(a, e.name, e.channelGroupType);
				//if (e.isPlaying)
				//{
				//	play(e.name, false);
				//	getAudio(e.name)->channel->setVolume(e.volume);
				//	getAudio(e.name)->channel->setPosition(e.position, FMOD_TIMEUNIT_MS);
				//}

			}
		}


		// !Serialization
		////
		//////
	private:
		std::map<std::string, AudioInstance> audios;
	};
}