#pragma once

#include <soloud.h>
#include <soloud_wav.h>
#include <Audio/ChannelGroupType.hpp>
#include <Utils/Dependency.hpp>
#include <memory>
#include <map>
#include <audio.hh>
#include <iostream>

class AudioManager : public std::enable_shared_from_this<AudioManager>, public Dependency<AudioManager>
{
public:
	AudioManager(void);
	virtual ~AudioManager(void);

	bool init();
	bool isInitialized();
	bool loadFile(std::string path, std::string soundName);
	bool loadMem(std::string path, std::string soundName);
	bool loadStream(std::string path, std::string soundName);
	float getVolume(void);
	void  setVolume(float volume);

	void play(std::string soundName);
	void pause(std::string soundName);
	void stop(std::string soundName);

	void setLooping(std::string soundName, bool looping);
	void setVolume(std::string soundName, float volume);
	void setSpeed(std::string soundName, float speed);
	void setPan(std::string soundName, float pan);

private:
	bool							_isInit;
	SoLoud::Soloud					_soloud;
	std::map<const std::string, Audio*>	_audios;
};