#pragma once

#if defined(__BORLANDC__)
# define AGE_COMPILER_BORLAND __BORLANDC__
#endif

#if defined(__COMO__)
# if defined(__COMO_VERSION__)
#  define AGE_COMPILER_COMEAU __COMO_VERSION__
# else
#  define AGE_COMPILER_COMEAU 0
# endif
#endif

#if defined(_CRAYC)
# if defined(_REVISION)
#  define AGE_COMPILER_CRAY _REVISION
# else
#  define AGE_COMPILER_CRAY 0
# endif
#endif

#if defined(__CYGWIN__)
# define AGE_COMPILER_CYGWIN 0
#endif

#if defined(__DECC) || defined(__DECCXX)
# if defined(__DECC_VER)
#  define AGE_COMPILER_DECC __DECC_VER
# else
#  define AGE_COMPILER_DECC 0
# endif
#else
# if defined(VAXC) || defined(__VAXC)
#  define AGE_COMPILER_DECC 0
# else
#  if defined(__osf__) && defined(__LANGUAGE_C__) && !defined(__GNUC__)
#   define AGE_COMPILER_DECC 0
#  endif
# endif
#endif

#if defined(__DCC__) && defined(__VERSION_NUMBER__)
# define AGE_COMPILER_DIAB __VERSION_NUMBER__
#endif

#if defined(__DMC__)
# define AGE_COMPILER_DMC __DMC__
#else
# if defined(__SC__) || defined(__ZTC__)
#  define AGE_COMPILER_DMC 0
# endif
#endif

#if defined(__GNUC__)
# if defined(__GNUC_PATCHLEVEL__)
#  define AGE_COMPILER_GCC ((__GNUC__ * 10000) + \
	(__GNUC_MINOR__ * 100) + \
	(__GNUC_PATCHLEVEL__))
# else
#  define AGE_COMPILER_GCC ((__GNUC__ * 10000) + \
	(__GNUC_MINOR__ * 100))
# endif
#endif

#if defined(__HP_aCC)
# if (__HP_aCC == 1)
#  define AGE_COMPILER_HPCC 11500
# else
#  define AGE_COMPILER_HPCC __HP_aCC
# endif
#else
# if defined(__HP_cc)
#  define AGE_COMPILER_HPCC __HP_cc
# else
#  if (__cplusplus >= 199707L) && defined(__hpux)
#   define AGE_COMPILER_HPCC 0
#  endif
# endif
#endif

#if defined(__INTEL_COMPILER)
# define AGE_COMPILER_INTEL __INTEL_COMPILER
#else
# if defined(__ICC)
#  define AGE_COMPILER_INTEL 0
# endif
#endif

#if defined(__KCC)
# if defined(__KCC_VERSION)
#  define AGE_COMPILER_KAI __KCC_VERSION
# else
#  define AGE_COMPILER_KAI 0
# endif
#endif

#if defined(__MINGW32__)
# define AGE_COMPILER_MINGW32 0
#endif

#if (defined(sgi) || defined(__sgi)) && defined(_COMPILER_VERSION)
# define AGE_COMPILER_MIPSPRO _COMPILER_VERSION
#endif

#if defined(__MRC__)
# define AGE_COMPILER_MPW __MRC__
#else
# if defined(MPW_C) || defined(MPW_CPLUS)
#  define AGE_COMPILER_MPW 0
# endif
#endif

#if defined(__MWERKS__)
# define AGE_COMPILER_MWERKS __MWERKS__
#endif

#if defined(__CC_NORCROFT)
# define AGE_COMPILER_NORCROFT 0
#endif

#if defined(_SCO_DS)
# define AGE_COMPILER_SCO 0
#endif

#if defined(__SUNPRO_C)
# define AGE_COMPILER_SUNPRO __SUNPRO_C
#else
# if defined(__SUNPRO_CC)
#  define AGE_COMPILER_SUNPRO __SUNPRO_CC
# endif
#endif

#if defined(__TenDRA__)
# define AGE_COMPILER_TENDRA 0
#endif

#if defined(__USLC__)
# if defined(__SCO_VERSION__)
#  define AGE_COMPILER_USLC __SCO_VERSION__
# else
#  define AGE_COMPILER_USLC 0
# endif
#endif

#if defined(_MSC_VER)
# define AGE_COMPILER_VISUALC _MSC_VER
# define AGE_VERSION_VISUALC_6_0 1200
# define AGE_VERSION_VISUALC_7_0 1300
#endif

#if defined(__WATCOMC__)
# define AGE_COMPILER_WATCOM __WATCOMC__
#endif

#if defined(__xlC__)
# define AGE_COMPILER_XLC __xlC__
#else
# if defined(__IBMC__) || defined(__IBMCPP__)
#  define AGE_COMPILER_XLC 0
# else
#  if defined(_AIX) && !defined(__GNUC__)
#   define AGE_COMPILER_XLC 0
#  endif
# endif
#endif