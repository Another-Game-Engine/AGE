#pragma once

#include <Components/Component.hh>
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>
#include <Audio/Audio.hh>
#include <Audio/ChannelGroupType.hpp>
#include <Utils/MatrixConversion.hpp>
#include <Audio/AudioManager.hh>
#include <Entities/EntityData.hh>
#include <Core/AScene.hh>
#include <vector>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

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

		struct SerializedAudioInstance
		{
			std::string filename;
			std::string name;
			ChannelGroupType channelGroupType;

			template <typename Archive>
			void serialize(Archive &ar)
			{
				ar(filename, name, channelGroupType);
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
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new AudioEmitter();
			res->setEntity(e);
			ar(*res);
			return res;
		}

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
				std::shared_ptr<Audio> a = _entity->getScene()->getInstance<AudioManager>()->getAudio(e.filename);
				if (!a)
					continue;
				setAudio(a, e.name, e.channelGroupType);
			}
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