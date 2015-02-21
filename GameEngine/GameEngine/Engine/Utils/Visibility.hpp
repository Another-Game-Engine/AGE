#pragma once

#if !defined(AGE_STATIC)
# define AGE_API_EXPORT __declspec(dllexport)
# define AGE_API_IMPORT __declspec(dllimport)
# ifdef _MSC_VER
#  pragma warning(disable: 4251)
# endif
#else
# define AGE_API_IMPORT
# define AGE_API_EXPORT
#endif

#if defined(AGE_EXPORT)
# define AGE_API AGE_API_EXPORT
#else
# define AGE_API AGE_API_IMPORT
#endif
