#include <Audio/AudioManager.hh>
#include <cassert>
#include <iostream>

AudioManager::AudioManager()
: _system(nullptr)
{}

AudioManager::~AudioManager()
{
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
	}
}

bool AudioManager::init()
{
	// FMOD IS ALREADY INTIALIZED
	if (_system != nullptr)
		return true;

	// CREATE SYSTEM
	if (!fmodError(FMOD::System_Create(&_system)))
		return false;

	// CHECK IF WE ARE USING THE SAME FMOD VERSION
	unsigned int version;
	if (!fmodError(_system->getVersion(&version)))
		return false;
	if (version != FMOD_VERSION)
	{
		std::cerr << "You are using an old version of FMOD : " << version << ". This program require version " << FMOD_VERSION << std::endl;
		return false;
	}

	int driverNumber;
	// GET THE NUMBER OF SOUND CARD
	if (!fmodError(_system->getNumDrivers(&driverNumber)))
		return false;
	if (driverNumber == 0)
	{
		// IF NO SOUND CARD THEN DISABLE SOUND
		if (fmodError(_system->setOutput(FMOD_OUTPUTTYPE_NOSOUND)))
			return false;
		return true;
	}

	// GET THE CAPABILITIES OF THE DEFAULT SOUND CARD
	if (!fmodError(_system->getDriverCaps(0, &_caps, 0, &_speakerMode)))
		return false;
	if (!fmodError(_system->setSpeakerMode(_speakerMode)))
		return false;

	// CHECK IF HARDWARE ACCELERATED IS ENABLED
	if (_caps & FMOD_CAPS_HARDWARE_EMULATED)
	{
		if (!fmodError(_system->setDSPBufferSize(1024, 10)))
			return false;
	}

	// SUPPORT SIGMATEL DRIVERS
	char name[256];
	if (!fmodError(_system->getDriverInfo(0, name, 256, 0)))
		return false;

	if (std::strstr(name, "SigmaTel"))
	{
		if (!fmodError(_system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR)))
			return false;
	}

	// INITIALIZE SYSTEM
	if (!fmodError(_system->init(1024, FMOD_INIT_NORMAL, 0)))
		return false;

	// create Music Channels

	for (std::size_t i = CHANNEL_GROUP_MUSIC; i <= CHANNEL_GROUP_CUSTOM_6; ++i)
	{
		FMOD::ChannelGroup *c;
		if (!fmodError(_system->createChannelGroup(NULL, &c)))
			return false;
		_channelGroups.insert(std::make_pair(static_cast<ChannelGroupType>(i), c));
	}

	// SET 1 3D listener
	if (!fmodError(_system->set3DNumListeners(1)))
		return false;
	return true;
}

void AudioManager::update()
{
	_system->update();
}

std::shared_ptr<Audio> AudioManager::loadSound(const File &file, Audio::AudioSpatialType spacialType, const std::string &name)
{
	std::string tname = name.empty() ? file.getShortFileName() : name;
	if (_audios.find(tname) != std::end(_audios))
		return _audios[name];

	std::shared_ptr<Audio> audio{ new Audio(shared_from_this(), file, Audio::AudioType::AUDIO_SOUND, name) };
	if (!audio->load(spacialType))
	{
//		std::cout << "Audio load failed : " << tname << std::endl;
		return nullptr;
	}
	_audios.insert(std::make_pair(tname, audio));
	return audio;
}

std::shared_ptr<Audio> AudioManager::loadStream(const File &file, Audio::AudioSpatialType spacialType, const std::string &name)
{
	std::string tname = name.empty() ? file.getShortFileName() : name;
	if (_audios.find(tname) != std::end(_audios))
		return _audios[name];
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
}