#include <Components/AudioEmitter.hpp>
#include <Entities/EntityData.hh>

using namespace Component;

//@ {
//Audio Instance


AudioEmitter::AudioInstance::AudioInstance()
: audio(nullptr)
, channel(nullptr)
, channelGroupType(CHANNEL_GROUP_CUSTOM_0)
, name("")
, reverb(nullptr)
{}

AudioEmitter::AudioInstance::~AudioInstance()
{
	reverb->release();
}

void AudioEmitter::AudioInstance::setReverb(FMOD_REVERB_PROPERTIES prop)
{
}

//@}

//@{
//Audio emitter

AudioEmitter::AudioEmitter() :
Component::ComponentBase<Component::AudioEmitter>()
{}

AudioEmitter::~AudioEmitter(void)
{}

void AudioEmitter::init()
{
	clearAllAudios();
}

void AudioEmitter::reset()
{
	clearAllAudios();
}

void AudioEmitter::clearAllAudios()
{
	for (auto &&e : audios)
		e.second.channel->stop();
	audios.clear();
}

void AudioEmitter::clearAudio(const std::string &name)
{
	if (audios.find(name) == std::end(audios))
		return;
	audios[name].channel->stop();
	audios.erase(name);
}

AudioEmitter::AudioInstance *AudioEmitter::getAudio(const std::string &name)
{
	if (audios.find(name) == std::end(audios))
		return nullptr;
	return &audios[name];
}

bool AudioEmitter::hasAudio(const std::string &name)
{
	if (audios.find(name) == std::end(audios))
		return false;
	return true;
}

void AudioEmitter::setAudio(std::shared_ptr<Audio> audio, const std::string &name, ChannelGroupType groupType)
{
	auto ai = hasAudio(name) ? audios[name] : AudioInstance();
	ai.channelGroupType = groupType;
	ai.name = name;
	ai.audio = audio;
	if (ai.channel)
		ai.channel->stop();
	ai.channel = nullptr;
	if (!hasAudio(name))
		audios.insert(std::make_pair(name, ai));
}

void AudioEmitter::updatePosition()
{
	for (auto &&e : audios)
	{
		if (!e.second.channel)
			continue;
		bool isPlaying;
		e.second.channel->isPlaying(&isPlaying);
		if (!isPlaying)
		{
			e.second.channel = nullptr;
			continue;
		}
		fmodError(e.second.channel->set3DMinMaxDistance(10, 100));
		glm::vec3 pos = posFromMat4(_entity->getGlobalTransform());
		FMOD_VECTOR  sourcePos = { pos.x, pos.y, pos.z };
		fmodError(e.second.channel->set3DAttributes(&sourcePos, 0));

	}
}

void AudioEmitter::play(const std::string &name, bool force)
{
	if (!hasAudio(name))
		return;
	auto a = getAudio(name);
	if (a->channel == nullptr || force == true)
	{
		if (a->channel)
			a->channel->stop();
		a->channel = a->audio->play(a->channelGroupType, true);
	}
	else if (a->channel != nullptr)
	{
		bool playing = true;
		a->channel->isPlaying(&playing);
		if (!playing)
		{
			a->channel = a->audio->play(a->channelGroupType, true);
		}
	}
}

//@}