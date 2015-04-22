#pragma once

#include <Utils/OpenGL.hh>

// Include that in preprocessor options
//_CRT_SECURE_NO_WARNINGS; MICROPROFILE_GPU_TIMERS = 1; MICROPROFILE_WEBSERVER = 1;

#define MICROPROFILE_MAX_FRAME_HISTORY (2<<10)
#define MICROPROFILE_IMPL
#define MICROPROFILE_GPU_TIMERS_GL 1
#include "microprofile.h"

MICROPROFILE_DEFINE(MAIN, "MAIN", "Main", 0xff0000);

// Call to this function at  the begining of the main
static void Age_microprofileInit()
{
	MicroProfileOnThreadCreate("Main");
	printf("Micro profile is enabled\n");

	//turn on profiling
	MicroProfileSetForceEnable(true);
	MicroProfileSetEnableAllGroups(true);
	MicroProfileSetForceMetaCounters(true);

	MicroProfileStartContextSwitchTrace();
}
