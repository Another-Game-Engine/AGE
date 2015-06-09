#pragma once

#include <map>
#include <iostream>

#include <soloud.h>
#include <soloud_wav.h>

#include <Utils/Dependency.hpp>
#include <Audio/audio.hh>

class AudioManager : public std::enable_shared_from_this<AudioManager>, public Dependency<AudioManager>
{
public:
	AudioManager(void);
	virtual ~AudioManager(void);

	bool init();
	bool isInitialized();
	bool loadFile(std::string const& path, std::string const& soundName);
	bool loadMem(std::string const& path, std::string const& soundName);
	bool loadStream(std::string const& path, std::string const& soundName);
	float getVolume(void);
	void  setVolume(float volume);

	void play(std::string const& soundName);
	void pause(std::string const& soundName);
	void stop(std::string const& soundName);

	void setLooping(std::string const& soundName, bool looping);
	void setVolume(std::string const& soundName, float volume);
	void setSpeed(std::string const& soundName, float speed);
	void setPan(std::string const& soundName, float pan);

private:
	bool							_isInit;
	SoLoud::Soloud					_soloud;
	std::map<const std::string, Audio*>	_audios;
};