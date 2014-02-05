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
	{}

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
		return true;
	}
private:
	FMOD::System *_system;
	FMOD_SPEAKERMODE _speakerMode;
	FMOD_CAPS _caps;
};