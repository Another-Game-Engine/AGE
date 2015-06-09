#include <Audio/AudioManager.hh>

AudioManager::AudioManager() :
	_isInit(false)
{}

AudioManager::~AudioManager()
{
	this->_isInit = false;
	this->_soloud.deinit();
}

bool AudioManager::init()
{
	this->_soloud.init();
	this->_isInit = true;
	return true;
}

bool AudioManager::isInitialized()
{
	return this->_isInit;
}

bool AudioManager::loadFile(std::string const& path, std::string const& soundName)
{
	SoLoud::Wav* sample = new SoLoud::Wav();
	auto res = sample->load(path.c_str());
	Audio* sound = new Audio(*sample);
	this->_audios.insert(std::pair<const std::string, Audio*>(soundName, sound));
	return true;
}

bool AudioManager::loadMem(std::string const& path, std::string const& soundName)
{
	std::cerr << "ERROR: The memberfunction AudioManager::loadMem() is not yet implemented | "
		<< __FILE__ << ":"
		<< __LINE__ << std::endl;
	return false;
}

bool AudioManager::loadStream(std::string const& path, std::string const& soundName)
{
	std::cerr << "ERROR: The memberfunction AudioManager::loadStream() is not yet implemented | "
		<< __FILE__ << ":"
		<< __LINE__ << std::endl;
	return false;
}

float AudioManager::getVolume(void)
{
	if (_isInit)
		return this->_soloud.getGlobalVolume();
	else
		return 0.f;
}

void AudioManager::setVolume(float volume)
{
	if (_isInit)
		this->_soloud.setGlobalVolume(volume);
}

void AudioManager::play(std::string const& soundName)
{
	if (_isInit)
	{
		auto sound = _audios.find(soundName)->second;
		if (sound->getState() == Audio::STOP)
			_soloud.play(sound->getSample(), sound->getVolume(), sound->getPan(), 0);
		if (sound->getState() == Audio::PAUSE)
			_soloud.setPause(sound->getHandle(), 0);
		sound->setState(Audio::PLAY);
	}
}

void AudioManager::pause(std::string const& soundName)
{
	if (_isInit)
	{
		auto sound = _audios.find(soundName)->second;
		if (sound->getState() == Audio::STOP)
			_soloud.play(sound->getSample(), sound->getVolume(), sound->getPan(), 1);
		if (sound->getState() == Audio::PLAY)
			_soloud.setPause(sound->getHandle(), 1);
		sound->setState(Audio::PAUSE);
	}
}

void AudioManager::stop(std::string const& soundName)
{
	if (_isInit)
	{
		auto sound = _audios.find(soundName)->second;
		_soloud.stop(sound->getHandle());
		sound->setState(Audio::STOP);
	}
}

void AudioManager::setLooping(std::string const& soundName, bool looping)
{
	auto sound = _audios.find(soundName)->second;
	if (sound->getState() != Audio::STOP)
		_soloud.setLooping(sound->getHandle(), looping);
	sound->setLooping(looping);
}

void AudioManager::setVolume(std::string const& soundName, float volume)
{
	auto sound = _audios.find(soundName)->second;
	if (sound->getState() != Audio::STOP)
		_soloud.setVolume(sound->getHandle(), volume);
	sound->setVolume(volume);
}

void AudioManager::setSpeed(std::string const& soundName, float speed)
{
	auto sound = _audios.find(soundName)->second;
	if (sound->getState() != Audio::STOP)
		_soloud.setRelativePlaySpeed(sound->getHandle(), speed);
	sound->setSpeed(speed);
}

void AudioManager::setPan(std::string const& soundName, float pan)
{
	auto sound = _audios.find(soundName)->second;
	if (sound->getState() != Audio::STOP)
		_soloud.setPan(sound->getHandle(), pan);
	sound->setPan(pan);
}