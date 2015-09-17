#pragma once

//// VERY IMPORTANT

// Include that in preprocessor options

//_CRT_SECURE_NO_WARNINGS; MICROPROFILE_GPU_TIMERS = 1; MICROPROFILE_WEBSERVER = 1;

// AND !!!

// link the project with Ws2_32.lib

#ifndef MICROPROFILE_ENABLED

#ifdef AGE_ENABLE_PROFILING
#define MICROPROFILE_ENABLED 1

#define MICROPROFILE_MAX_FRAME_HISTORY (2<<10)
#define MICROPROFILE_GPU_TIMERS_GL 1
#ifndef _CRT_SECURE_NO_WARNINGS
# define _CRT_SECURE_NO_WARNINGS
#endif
#define MICROPROFILE_GPU_TIMERS 1
#define MICROPROFILE_WEBSERVER 1

#else
#define MICROPROFILE_ENABLED 0
#endif

#endif


// Call to this function at  the begining of the main
void Age_microprofileInit();
//const char* MicroProfileGetThreadName();
