#pragma once

#include <Utils/File.hpp>
#include <fmod.hpp>
#include <memory>
#include <cassert>
#include <Audio/FmodError.hpp>
#include <Audio/ChannelGroupType.hpp>

class AudioManager;

class Audio
{
public:
	enum AudioType
	{
		AUDIO_TYPE_UNDEFINED = 0
		, AUDIO_SOUND = 1
		, AUDIO_STREAM = 2
	};

	enum AudioSpatialType
	{
		AUDIO_SPACE_UNDEFINED = 0
		, AUDIO_2D = FMOD_2D
		, AUDIO_3D = FMOD_3D
	};

public:
	Audio(AudioManager *manager, const File &file, AudioType type, const std::string &name = "");
	~Audio();
	bool load(AudioSpatialType type);
	void play(ChannelGroupType channelGroup);
	FMOD::Channel *getChannel();
private:
	AudioManager *_manager;
	File _file;
	const std::string _name;
	FMOD::Sound *_audio;
	AudioType _audioType;
	FMOD::Channel *_channel;
};