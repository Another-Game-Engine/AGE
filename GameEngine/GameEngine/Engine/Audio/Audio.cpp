#include "Audio.hh"

Audio::Audio(SoLoud::Wav & sample) :
_sample(sample),
_volume(1.0f),
_speed(1.0f),
_pan(1.0f),
_looping(false),
_handle(0),
_state(STOP)
{
}

Audio::~Audio()
{
}

SoLoud::Wav & Audio::getSample(void) const
{
	return this->_sample;
}

float Audio::getVolume(void) const
{
	return this->_volume;
}

float Audio::getSpeed(void) const
{
	return this->_speed;
}

float Audio::getPan(void) const
{
	return this->_pan;
}

int	Audio::getHandle(void) const
{
	return this->_handle;
}

bool Audio::getLooping(void) const
{
	return this->_looping;
}

void Audio::setSample(SoLoud::Wav & other)
{
	this->_sample = other;
}

void Audio::setVolume(float other)
{
	this->_volume = other;
}

void Audio::setSpeed(float other)
{
	this->_speed = other;
}

void Audio::setPan(float other)
{
	this->_pan = other;
}

void Audio::setHandle(int other)
{
	this->_handle = other;
}

void Audio::setLooping(bool other)
{
	this->_looping = other;
}