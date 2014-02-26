#include <Audio/Audio.hh>
#include <Audio/AudioManager.hh>

Audio::Audio(std::shared_ptr<AudioManager> manager, const File &file, AudioType type, const std::string &name)
: _manager(manager)
, _file(file)
, _name(name.empty() ? file.getShortFileName() : name)
, _audio(nullptr)
, _audioType(type)
{
	assert(type != AUDIO_TYPE_UNDEFINED && "Audio type is undefined");
	assert(manager != nullptr && "Audio manager is null");
}

Audio::~Audio()
{
	if (!_audio)
		return;
	_audio->release();
}

bool Audio::load(AudioSpatialType type)
{
	if (_audio)
		return true;
	FMOD::Sound *audio;
	if (_audioType == AUDIO_SOUND)
	{
		_manager->getSystem()->createSound(_file.getFullName().c_str(), type, 0, &audio);
	}
	else if (_audioType == AUDIO_STREAM)
	{
		_manager->getSystem()->createStream(_file.getFullName().c_str(), type, 0, &audio);
	}
	if (audio == nullptr)
	{
		std::cerr << "FMOD failed to load audio." << std::endl;
		return false;
	}
	_audio = std::shared_ptr<FMOD::Sound>(audio);
	return true;
}

const File &Audio::getPath() const
{
	return _file;
}

const std::string &Audio::getName() const
{
	return _name;
}

FMOD::Channel *Audio::play(ChannelGroupType channelGroup, bool now)
{
	FMOD::Channel *channel = nullptr;
	_manager->getSystem()->playSound(FMOD_CHANNEL_FREE, _audio.get(), true, &channel);
	channel->setChannelGroup(_manager->getChannelGroup(channelGroup));
	channel->setPaused(!now);
	return channel;
}