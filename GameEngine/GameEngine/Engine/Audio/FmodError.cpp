#include <Audio/FmodError.hpp>

bool fmodError(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cerr << "FMOD error : [" << result << "] :: " << FMOD_ErrorString(result) << std::endl;
		return false;
	}
	return true;
}