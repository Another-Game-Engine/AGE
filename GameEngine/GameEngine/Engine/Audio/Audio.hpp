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
	Audio(const File &file, AudioType type, const std::string &name = "")
		: _file(file)
		, _name(name.empty() ? file.getShortFileName() : name)
		, _audio(nullptr)
		, _audioType(type)
	{
		assert(type != AUDIO_TYPE_UNDEFINED && "Audio type is undefined");
	}

	~Audio()
	{}

	bool load(AudioSpatialType type)
	{
		if (_audio)
			return true;
		return true;
	}

private:
	File _file;
	const std::string _name;
	std::shared_ptr<FMOD::Sound> _audio;
	AudioType _audioType;
};