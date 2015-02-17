#pragma once

#include "Compiler.hpp"

#if defined(__amigaos__) \
	|| defined(AMIGA)
# define AGE_OS_AMIGA
#endif

#if defined(_AIX) \
	|| defined(__TOS_AIX__) \
	|| defined(AGE_COMPILER_XLC)
# define AGE_OS_AIX
#endif

#if defined(__BEOS__)
# define AGE_OS_BEOS
#endif

#if defined(bsdi) \
	|| defined(__bsdi__)
# define AGE_OS_BSDI
#endif

#if defined(_UNICOS) \
	|| defined(_CRAY)
# define AGE_OS_CRAY
#endif

#if defined(DGUX) \
	|| defined(__DGUX__) \
	|| defined(__dgux__)
# define AGE_OS_DGUX
#endif

#if defined(__DragonFly__)
# define AGE_OS_DRAGONFLY
#endif

#if defined(__FreeBSD__) \
	|| defined(__FreeBSD_kernel__)
# define AGE_OS_FREEBSD
#endif

#if defined(hpux) \
	|| defined(__hpux) \
	|| defined(_HPUX_SOURCE)
# define AGE_OS_HPUX
#endif

#if defined(__GNU__)
# define AGE_OS_HURD
#endif

#if defined(sgi) \
	|| defined(__sgi) \
	|| defined(mips) \
	|| defined(_SGI_SOURCE)
# define AGE_OS_IRIX
#endif

#if defined(linux) \
	|| defined(__linux) \
	|| defined(__linux__)
# define AGE_OS_LINUX
#endif

#if defined(macintosh) \
	|| defined(Macintosh) \
	|| defined(__MACOS__)
# define AGE_OS_MACINTOSH
#endif

#if defined(__MACOSX__) \
	|| defined(MACOSX) \
	|| (defined(__APPLE__) && defined(__MACH__))
# define AGE_OS_MACOSX
#endif

#if defined(mpeix) \
	|| defined(__mpexl)
# define AGE_OS_MPEIX
#endif

#if defined(MSDOS) \
	|| defined(__MSDOS__) \
	|| defined(_MSDOS) \
	|| defined(__DOS__)
# define AGE_OS_MSDOS
#endif

#if defined(__NetBSD__)
# define AGE_OS_NETBSD
#endif

#if defined(__OpenBSD__)
# define AGE_OS_OPENBSD
#endif

#if defined(OS2) \
	|| defined(_OS2) \
	|| defined(__OS2__) \
	|| defined(__TOS_OS2__)
# define AGE_OS_OS2
#endif

#if defined(__osf__) \
	|| defined(__osf) \
	|| defined(AGE_COMPILER_DECC)
# define AGE_OS_OSF
#endif

#if defined(pyr)
# define AGE_OS_PYRAMID
#endif

#if defined(__QNX__) \
	|| defined(__QNXNTO__)
# define AGE_OS_QNX
#endif

#if defined(M_I386) \
	|| defined(M_XENIX) \
	|| defined(_SCO_C_DIALECT) \
	|| defined(AGE_COMPILER_SCO)
# define AGE_OS_SCO
#endif

#if defined(_SEQUENT_) \
	|| defined(sequent)
# define AGE_OS_SEQUENT
#endif

#if defined(sinix)
# define AGE_OS_SINIX
#endif

#if defined(sun) \
	|| defined(__sun__) \
	|| defined(AGE_COMPILER_SUNPRO)
# if defined(__SVR4) || defined(__svr4__)
#  define AGE_OS_SOLARIS
# else
#  define AGE_OS_SUNOS
# endif
#endif

#if defined(__SYMBIAN32__)
# define AGE_OS_SYMBIAN
#endif

#if defined(ultrix) \
	|| defined(__ultrix) \
	|| defined(__ultrix__)
# define AGE_OS_ULTRIX
#endif

#if defined(VMS) \
	|| defined(__VMS)
# undef AGE_OS_OSF
# define AGE_PLATFORM_VMS
# if defined(__VMS_VER)
#  define AGE_OS_VMS __VMS_VER
# else
#  define AGE_OS_VMS 0
# endif
#endif

#if defined(__ANDROID__) \
	|| defined(ANDROID) \
	|| defined(__ANDROID)
# define AGE_PLATFORM_ANDROID
#endif

#if defined(AGE_OS_MACINTOSH) \
	|| defined(AGE_OS_MACOSX)
# if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#  define AGE_PLATFORM_IOS
# else
#  define AGE_PLATFORM_MACOS
# endif
#endif

#if defined(unix) \
	|| defined(__unix) \
	|| defined(__unix__)
# define AGE_PLATFORM_LINUX
#endif
#if defined(AGE_OS_AIX) \
	|| defined(AGE_OS_OSF) \
	|| defined(AGE_OS_NETBSD) \
	|| defined(AGE_OS_QNX)
# if !defined(AGE_PLATFORM_LINUX)
#  define AGE_PLATFORM_LINUX
# endif
#endif
#if defined(AGE_COMPILER_CYGWIN)
# define AGE_PLATFORM_LINUX
#endif
#if defined(AMIGA) && defined(AGE_COMPILER_GCC)
# define AGE_PLATFORM_LINUX
#endif

#if defined(PS3) \
	|| defined(_PS3) \
	|| defined(SN_TARGET_PS3) \
	|| defined(XENON) \
	|| defined(XENON_INTRINSICS)
# define AGE_PLATFORM_PS3
#endif

#if defined(XBOX) \
	|| defined(_XBOX)
# define AGE_PLATFORM_XBOX360
#endif

#if defined(WIN32) \
	|| defined(_WIN32) \
	|| defined(__WIN32__) \
	|| defined(WIN64) \
	|| defined(_WIN64) \
	|| defined(__WIN64__) \
	|| defined(__TOS_WIN__) \
	|| defined(AGE_COMPILER_VISUALC)
# define AGE_PLATFORM_WINDOWS
# define AGE_OS_WINDOWS
#endif

#if defined(AGE_OS_FREEBSD) \
	|| defined(AGE_OS_NETBSD) \
	|| defined(AGE_OS_OPENBSD) \
	|| defined(AGE_OS_BSDI) \
	|| defined(AGE_OS_DRAGONFLY) \
	|| defined(_BSD_SOURCE) \
	|| defined(_SYSTYPE_BSD)
# define AGE_PLATFORM_BSD
#endif

#if defined(__sysv__) \
	|| defined(__SVR4) \
	|| defined(__svr4__) \
	|| defined(_SVR4_SOURCE) \
	|| defined(_SYSTYPE_SVR4)
# define AGE_PLATFORM_SVR4
#endif

#if defined(UWIN)
# define AGE_PLATFORM_UWIN
#endif

#if defined(_WINDU_SOURCE)
# define AGE_PLATFORM_WINDU
#endif

#if defined(AGE_PLATFORM_WINDOWS)
# define NOMINMAX
# include <Windows.h>
#endif

#if !defined(AGE_PLATFORM_WINDOWS) \
	&& !defined(AGE_PLATFORM_LINUX) \
	&& !defined(AGE_PLATFORM_MACOS) \
	&& !defined(AGE_PLATFORM_IOS) \
	&& !defined(AGE_PLATFORM_ANDROID) \
	&& !defined(AGE_PLATFORM_PS3) \
	&& !defined(AGE_PLATFORM_XBOX360)
# error "Unsupported platform"
#endif