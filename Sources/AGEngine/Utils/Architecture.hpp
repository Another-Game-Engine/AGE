#pragma once

#include "Platform.hpp"

#if defined(AGE_PLATFORM_WINDOWS)
# if defined(WIN64) \
    || defined(_WIN64) \
    || defined(__WIN64__)
#  define AGE_64_BIT
# else
#  define AGE_32_BIT
# endif
#else
# if defined(__x86_64__) \
	|| defined(__ppc64__) \
	|| defined(_LP64) \
	|| defined(_lp64) \
    || defined(__LP64__) \
    || defined(__lp64__)
#  define AGE_64_BIT
# else
#  define AGE_32_BIT
# endif
#endif