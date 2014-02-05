#pragma once

#include <Utils/File.hpp>
#include <fmod.hpp>
#include <memory>
#include <cassert>

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
	Audio(FMOD::System *system, const File &file, AudioType type, const std::string &name = "")
		: _system(system)
		, _file(file)
		, _name(name.empty() ? file.getShortFileName() : name)
		, _audio(nullptr)
		, _audioType(type)
	{
		assert(type != AUDIO_TYPE_UNDEFINED && "Audio type is undefined");
	}

	~Audio()
	{
		if (!_audio)
			return;
		_audio->release();
	}

	bool load(AudioSpatialType type)
	{
		if (_audio)
			return true;
		if (_audioType == AUDIO_SOUND)
		{
			_system->createSound(_file.getFullName().c_str(), type, 0, &_audio);
		}
		else if (_audioType == AUDIO_STREAM)
		{
			_system->createStream(_file.getFullName().c_str(), type, 0, &_audio);
		}
		assert(_audio != nullptr && "FMOD failed to load audio.");
		return true;
	}

	void play()
	{
		_system->playSound(FMOD_CHANNEL_FREE, _audio, false, 0);
	}

private:
	FMOD::System *_system;
	File _file;
	const std::string _name;
	FMOD::Sound *_audio;
	AudioType _audioType;
};