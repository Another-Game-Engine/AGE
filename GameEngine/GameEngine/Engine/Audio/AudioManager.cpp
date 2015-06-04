#include <Audio/AudioManager.hh>
#include <cassert>
#include <iostream>

AudioManager::AudioManager()
{}

AudioManager::~AudioManager()
{
	this->_soloud.deinit();
	/*
	_audios.clear();
	for (auto &&e : _channelGroups)
	{
		e.second->release();
	}
	_channelGroups.clear();
	if (_system)
	{
		_system->close();
		_system->release();
	}*/
}

bool AudioManager::init()
{
	this->_soloud.init();
	return true;
}
/*
void AudioManager::update()
{
	_system->update();
}

std::shared_ptr<Audio> AudioManager::loadSound(const OldFile &file, Audio::AudioSpatialType spacialType, const std::string &name)
{
	std::string tname = name.empty() ? file.getShortFileName() : name;
	if (_audios.find(tname) != std::end(_audios))
		return _audios[tname];

	std::shared_ptr<Audio> audio{ new Audio(shared_from_this(), file, Audio::AudioType::AUDIO_SOUND, name) };
	if (!audio->load(spacialType))
	{
//		std::cout << "Audio load failed : " << tname << std::endl;
		return nullptr;
	}
	_audios.insert(std::make_pair(tname, audio));
	return audio;
}

std::shared_ptr<Audio> AudioManager::loadStream(const OldFile &file, Audio::AudioSpatialType spacialType, const std::string &name)
{
	std::string tname = name.empty() ? file.getShortFileName() : name;
	if (_audios.find(tname) != std::end(_audios))
		return _audios[tname];
	std::shared_ptr<Audio> audio{ new Audio(shared_from_this() , file, Audio::AudioType::AUDIO_STREAM, name) };
	if (!audio->load(spacialType))
	{
//		std::cout << "Audio load failed : " << tname << std::endl;
		return nullptr;
	}
	_audios.insert(std::make_pair(tname, audio));
	return audio;
}

FMOD::System *AudioManager::getSystem()
{
	return _system;
}

FMOD::ChannelGroup *AudioManager::getChannelGroup(ChannelGroupType type)
{
	assert(_channelGroups.find(type) != std::end(_channelGroups) && "Channel goup not found.");
	return _channelGroups[type];
}*/