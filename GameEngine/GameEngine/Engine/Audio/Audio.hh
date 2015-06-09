#pragma once

#include <soloud_wav.h>

class Audio
{
public:
	enum SoundPlayState
	{
		STOP = 0,
		PLAY = 1,
		PAUSE = 2
	};

	Audio(SoLoud::Wav & sample);
	~Audio();

	SoLoud::Wav & getSample(void) const;
	float getVolume(void) const;
	float getSpeed(void) const;
	float getPan(void) const;
	bool  getLooping(void) const;
	int	  getHandle(void) const;
	SoundPlayState getState(void) const;

	void setSample(SoLoud::Wav & other);
	void setVolume(float other);
	void setSpeed(float other);
	void setPan(float other);
	void setLooping(bool other);
	void setHandle(int other);
	void setState(SoundPlayState other);

private:
	SoLoud::Wav &_sample;
	float		_volume;
	float		_speed;
	float		_pan;
	bool		_looping;
	int			_handle;
	SoundPlayState _state;
};

