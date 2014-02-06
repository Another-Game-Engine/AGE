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
			AudioInstance()
				: audio(nullptr)
				, channel(nullptr)
				, channelGroupType(CHANNEL_GROUP_CUSTOM_0)
				, name("")
			{}
			std::shared_ptr<Audio> audio;
			FMOD::Channel *channel;
			ChannelGroupType channelGroupType;
			std::string name;
		};

		AudioEmitter() :
			Component::ComponentBase<Component::AudioEmitter>()
		{}
		virtual ~AudioEmitter(void)
		{}

		void init()
		{
			clearAllAudios();
		}

		virtual void reset()
		{
			clearAllAudios();
		}

		void clearAllAudios()
		{
			for (auto &&e : audios)
				e.second.channel->stop();
			audios.clear();
		}

		void clearAudio(const std::string &name)
		{
			if (audios.find(name) == std::end(audios))
				return;
			audios[name].channel->stop();
			audios.erase(name);
		}

		AudioInstance *getAudio(const std::string &name)
		{
			if (audios.find(name) == std::end(audios))
				return nullptr;
			return &audios[name];
		}

		bool hasAudio(const std::string &name)
		{
			if (audios.find(name) == std::end(audios))
				return false;
			return true;
		}

		void setAudio(std::shared_ptr<Audio> audio, const std::string &name, ChannelGroupType groupType)
		{
			auto ai = hasAudio(name) ? audios[name] : AudioInstance();
			ai.channelGroupType = groupType;
			ai.name = name;
			ai.audio = audio;
			if (ai.channel)
				ai.channel->stop();
			ai.channel = nullptr;
			if (!hasAudio(name))
				audios.insert(std::make_pair(name, ai));
		}

		void updatePosition()
		{
			for (auto &&e : audios)
			{
				if (!e.second.channel)
					continue;
				bool isPlaying;
				e.second.channel->isPlaying(&isPlaying);
				if (!isPlaying)
				{
					e.second.channel = nullptr;
					continue;
				}
//				fmodError(e.second.channel->set3DMinMaxDistance(10, 100));
				glm::vec3 pos = posFromMat4(_entity->getGlobalTransform());
				FMOD_VECTOR  sourcePos = { pos.x, pos.y, pos.z };
				fmodError(e.second.channel->set3DAttributes(&sourcePos, 0));

			}
		}

		void play(const std::string &name, bool force)
		{
			if (!hasAudio(name))
				return;
			auto a = getAudio(name);
			if (a->channel == nullptr || force == true)
			{
				if (a->channel)
					a->channel->stop();
				a->channel = a->audio->play(a->channelGroupType, true);
			}
			else if (a->channel != nullptr)
			{
				bool playing = true;
				a->channel->isPlaying(&playing);
				if (!playing)
				{
					a->channel = a->audio->play(a->channelGroupType, true);
				}
			}
		}

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
		std::map<std::string, AudioInstance> audios;
		AudioEmitter(AudioEmitter const &);
		AudioEmitter &operator=(AudioEmitter const &);
	};
}