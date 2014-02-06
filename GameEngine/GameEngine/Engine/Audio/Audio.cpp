#include <Audio/Audio.hh>
#include <Audio/AudioManager.hh>

Audio::Audio(AudioManager *manager, const File &file, AudioType type, const std::string &name)
: _manager(manager)
, _file(file)
, _name(name.empty() ? file.getShortFileName() : name)
, _audio(nullptr)
, _audioType(type)
, _channel(nullptr)
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
	if (_audioType == AUDIO_SOUND)
	{
		_manager->getSystem()->createSound(_file.getFullName().c_str(), type, 0, &_audio);
	}
	else if (_audioType == AUDIO_STREAM)
	{
		_manager->getSystem()->createStream(_file.getFullName().c_str(), type, 0, &_audio);
	}
	assert(_audio != nullptr && "FMOD failed to load audio.");
	return true;
}

void Audio::play(ChannelGroupType channelGroup)
{
	_channel = nullptr;
	_manager->getSystem()->playSound(FMOD_CHANNEL_FREE, _audio, true, &_channel);
	fmodError(_channel->set3DMinMaxDistance(10, 100));
	FMOD_VECTOR  sourcePos = { 0.0f, 0.0f, 0.0f };
	_channel->set3DAttributes(&sourcePos, 0);
	_channel->setPaused(false);
	_channel->setChannelGroup(_manager->getChannelGroup(channelGroup));
}

FMOD::Channel *Audio::getChannel()
{
	return _channel;
}