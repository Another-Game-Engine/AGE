#include "Age_microprofile.hpp"
#include <string>

// Don't forget to define MICROPROFILE_IMPL in a .cpp
#define MICROPROFILE_IMPL
#define MICROPROFILE_GPU_TIMERS_GL 1

#include <microprofile/microprofile.h>

MICROPROFILE_DEFINE(MAIN, "MAIN", "Main", 0xff0000);

#ifdef AGE_ENABLE_PROFILING

void Age_microprofileInit()
{
	MicroProfileOnThreadCreate("Main");
	printf("Micro profile is enabled\n");

	//turn on profiling
	MicroProfileSetForceEnable(true);
	MicroProfileSetEnableAllGroups(true);
	MicroProfileSetForceMetaCounters(true);

	MicroProfileStartContextSwitchTrace();
}

//const char *MicroProfileGetThreadName()
//{
//	stringstream ss << std::this_thread::get_id();
//	return ss.str().c_str();
//}

#else

void Age_microprofileInit(){}

#endif