#pragma once

#include <cassert>
#include <cstdlib>

#include "Platform.hpp"
#include "Logger.hpp"

#if defined(AGE_PLATFORM_PS3)
# define LIKELY(expression) __builtin_expect((expression), 1)
# define UNLIKELY(expression) __builtin_expect((expression), 0)
#else
# define LIKELY(expression) (expression)
# define UNLIKELY(expression) (expression)
#endif

#define AGE_FILE __FILE__
#define AGE_LINE __LINE__

#define AGE_SUCCESS EXIT_SUCCESS
#define AGE_FAILURE EXIT_FAILURE

#define AGE_EXIT(ErrorCode) std::exit(ErrorCode)

#if defined(_DEBUG) || defined(DEBUG)
# define AGE_DEBUG
#endif

#if !defined(AGE_DEBUG) && !defined(NDEBUG)
# define NDEBUG
#endif

#if !defined(AGE_DEBUG)
# define AGE_RELEASE
#endif

#if defined(AGE_DEBUG)
# if defined(AGE_PLATFORM_WINDOWS)
#  define AGE_FUNCTION __FUNCTION__
# else
#  define AGE_FUNCTION __func__
# endif
#else
# define AGE_FUNCTION "N/A"
#endif

#if defined(AGE_DEBUG)
# if defined(AGE_PLATFORM_WINDOWS)
#  define AGE_BREAK() __debugbreak()
# elif defined(AGE_PLATFORM_LINUX) || defined(AGE_PLATFORM_MACOS)
#  define AGE_BREAK() __builtin_trap()
# else
#  define AGE_BREAK()
# endif
#else
# define AGE_BREAK()
#endif

#if defined(AGE_DEBUG)

# define AGE_LOG(...) do \
		{ \
	Singleton<AGE::Logger>::getInstance()->log(AGE::Logger::Level::Normal, "Function \"", AGE_FUNCTION, "\" in file \"", AGE_FILE, "\" (line ", AGE_LINE, "): ", __VA_ARGS__); \
		} \
			while (false);

# define AGE_WARNING(...) do \
		{ \
	Singleton<AGE::Logger>::getInstance()->log(AGE::Logger::Level::Warning, "Function \"", AGE_FUNCTION, "\" in file \"", AGE_FILE, "\" (line ", AGE_LINE, "): ", __VA_ARGS__); \
	AGE_BREAK(); \
		} \
			while (false);

# define AGE_ERROR(...) do \
		{ \
	Singleton<AGE::Logger>::getInstance()->log(AGE::Logger::Level::Error, "Function \"", AGE_FUNCTION, "\" in file \"", AGE_FILE, "\" (line ", AGE_LINE, "): ", __VA_ARGS__); \
	AGE_BREAK(); \
		} \
			while (false);

# define AGE_FATAL(...) do \
		{ \
	Singleton<AGE::Logger>::getInstance()->log(AGE::Logger::Level::Fatal, "Function \"", AGE_FUNCTION, "\" in file \"", AGE_FILE, "\" (line ", AGE_LINE, "): ", __VA_ARGS__); \
	AGE_BREAK(); \
	AGE_EXIT(AGE_FAILURE); \
		} \
			while (false);

#else

# define AGE_LOG(...) do \
		{ \
	Singleton<AGE::Logger>::getInstance()->log(AGE::Logger::Level::Normal, __VA_ARGS__); \
		} \
			while (false);

# define AGE_WARNING(...) do \
		{ \
	Singleton<AGE::Logger>::getInstance()->log(AGE::Logger::Level::Warning, __VA_ARGS__); \
		} \
			while (false);

# define AGE_ERROR(...) do \
		{ \
	Singleton<AGE::Logger>::getInstance()->log(AGE::Logger::Level::Error, __VA_ARGS__); \
		} \
			while (false);

# define AGE_FATAL(...) do \
		{ \
	Singleton<AGE::Logger>::getInstance()->log(AGE::Logger::Level::Fatal, __VA_ARGS__); \
		} \
			while (false);

#endif

#if defined(AGE_DEBUG)
# define AGE_ASSERT(condition) if (!UNLIKELY(condition)) AGE_FATAL(#condition)
# define AGE_ASSERT_EQUAL(x, y) AGE_ASSERT((x) == (y))
# define AGE_ASSERT_DIFFERENT(x, y) AGE_ASSERT((x) != (y))
# define AGE_ASSERT_LESS(x, y) AGE_ASSERT((x) < (y))
# define AGE_ASSERT_LESS_OR_EQUAL(x, y) AGE_ASSERT((x) <= (y))
# define AGE_ASSERT_GREATER(x, y) AGE_ASSERT((x) > (y))
# define AGE_ASSERT_GREATER_OR_EQUAL(x, y) AGE_ASSERT((x) >= (y))
#else
# define AGE_ASSERT(condition)
# define AGE_ASSERT_EQUAL(x, y)
# define AGE_ASSERT_DIFFERENT(x, y)
# define AGE_ASSERT_LESS(x, y)
# define AGE_ASSERT_LESS_OR_EQUAL(x, y)
# define AGE_ASSERT_GREATER(x, y)
# define AGE_ASSERT_GREATER_OR_EQUAL(x, y)
#endif