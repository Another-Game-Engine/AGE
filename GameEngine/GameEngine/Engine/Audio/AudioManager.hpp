#pragma once

#include <fmod.hpp>
#include <Audio/FmodError.hpp>
#include <Utils/Dependency.hpp>
#include <memory>

class AudioManager : public Dependency
{
public:
	AudioManager()
		: _system(nullptr)
	{}

	virtual ~AudioManager()
	{
		if (_system)
		{
			_system->release();
			delete _system;
		}
	}

	bool init()
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
		if (!fmodError(_system->init(32, FMOD_INIT_NORMAL, 0)))
			return false;
		return true;
	}
private:
	FMOD::System *_system;
	FMOD_SPEAKERMODE _speakerMode;
	FMOD_CAPS _caps;
};