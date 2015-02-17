#pragma once

#define AGE_LITTLE_ENDIAN_BYTE_ORDER 1234
#define AGE_BIG_ENDIAN_BYTE_ORDER 4321

#if defined(__linux__)
# include <endian.h>
# define AGE_BYTE_ORDER __BYTE_ORDER
#else
# if defined(__hppa__) || \
    defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MISPEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
    defined(__sparc__)
#  define AGE_BYTE_ORDER AGE_BIG_ENDIAN_BYTE_ORDER
# else
#  define AGE_BYTE_ORDER AGE_LITTLE_ENDIAN_BYTE_ORDER
# endif
#endif

#if AGE_BYTE_ORDER == AGE_LITTLE_ENDIAN_BYTE_ORDER
# define AGE_LITTLE_ENDIAN
#else
# define AGE_BIG_ENDIAN
#endif