#pragma once

#include <fmod.hpp>
#include <Audio/FmodError.hpp>
#include <Audio/ChannelGroupType.hpp>
#include <Utils/Dependency.hpp>
#include <memory>
#include <map>
#include <iostream>
#include <Audio/Audio.hh>

class AudioManager : public Dependency
{
public:
	AudioManager();
	virtual ~AudioManager();
	bool init();
	void update();
	std::shared_ptr<Audio> loadSound(const File &file, Audio::AudioSpatialType spacialType, const std::string &name = "");
	std::shared_ptr<Audio> loadStream(const File &file, Audio::AudioSpatialType spacialType, const std::string &name = "");
	FMOD::System *getSystem();
	FMOD::ChannelGroup* getChannelGroup(ChannelGroupType type);
	inline std::shared_ptr<Audio> getAudio(const std::string &name){ auto r = _audios.find(name); if (r == std::end(_audios)) return nullptr; return r->second; }
private:
	FMOD::System *_system;
	FMOD_SPEAKERMODE _speakerMode;
	FMOD_CAPS _caps;
	std::map<ChannelGroupType, FMOD::ChannelGroup*> _channelGroups;
	std::map<std::string, std::shared_ptr<Audio>> _audios;
};