#pragma once

#include <cstdint>

#ifdef AGE_ENABLE_PROFILING
#include <microprofile/microprofile.h>
#endif

namespace AGE
{
	struct Profiler
	{
		static std::uint32_t GetColor(const char* str);
	};

#ifdef AGE_ENABLE_PROFILING
	// Defines a scope for CPU task
	// Call SCOPE_profile_cpu(name) to use it
#define DEFINE_profile_cpu(name, groupName, scopeName) \
  MICROPROFILE_DEFINE(name, groupName, scopeName,      \
                      AGE::Profiler::GetColor(scopeName))

	// Declares a previously defined profile scope. Use in a translation unit.
#define DECLARE_profile_cpu(name) MICROPROFILE_DECLARE(name)

	// Defines a profiling scope for GPU tasks.
	// Use `COUNT_profile_gpu(name)` to activate the scope.
#define DEFINE_profile_gpu(name, groupName, scopeName) \
  MICROPROFILE_DEFINE_GPU(name, groupName, scopeName,  \
                          AGE::Profiler::GetColor(scopeName))

	// Declares a previously defined profile scope. Use in a translation unit.
#define DECLARE_profile_gpu(name) MICROPROFILE_DECLARE_GPU(name)

	// Enters a previously defined CPU profiling scope, active for the duration
	// of the containing block.
#define SCOPE_profile_cpu(name) MICROPROFILE_SCOPE(name)

	// Enters a CPU profiling scope, active for the duration of the containing
	// block. No previous definition required.
#define SCOPE_profile_cpu_i(groupName, scopeName) \
  MICROPROFILE_SCOPEI(groupName, scopeName,       \
                      AGE::Profiler::GetColor(scopeName))

	// Enters a CPU profiling scope by function name, active for the duration of
	// the containing block. No previous definition required.
#define SCOPE_profile_cpu_function(groupName)         \
  MICROPROFILE_SCOPEI(groupName, __FUNCTION__, \
                      AGE::Profiler::GetColor(__FUNCTION__))

	// Enters a previously defined GPU profiling scope, active for the duration
	// of the containing block.
#define SCOPE_profile_gpu(name) MICROPROFILE_SCOPEGPU(name)

	// Enters a GPU profiling scope, active for the duration of the containing
	// block. No previous definition required.
#define SCOPE_profile_gpu_i(scopeName) \
  MICROPROFILE_SCOPEGPUI(scopeName,    \
                         AGE::Profiler::GetColor(scopeName))

	// Enters a GPU profiling scope by function name, active for the duration of
	// the containing block. No previous definition required.
#define SCOPE_profile_gpu_function(groupName)            \
  MICROPROFILE_SCOPEGPUI(groupName, __FUNCTION__, \
                         AGE::Profiler::GetColor(__FUNCTION__))

	// Tracks a CPU value counter.
#define COUNT_profile_cpu(name, count) MICROPROFILE_META_CPU(name, count)

	// Tracks a GPU value counter.
#define COUNT_profile_gpu(name, count) MICROPROFILE_META_GPU(name, count)
#else

#define DEFINE_profile_cpu(name, groupName, scopeName)
#define DEFINE_profile_gpu(name, groupName, scopeName)
#define DECLARE_profile_cpu(name)
#define DECLARE_profile_gpu(name)
#define SCOPE_profile_cpu(name) \
  do {                          \
      } while (false)
#define SCOPE_profile_cpu_function(name) \
  do {                            \
      } while (false)
#define SCOPE_profile_cpu_i(groupName, scopeName) \
  do {                                              \
      } while (false)
#define SCOPE_profile_gpu(name) \
  do {                          \
      } while (false)
#define SCOPE_profile_gpu_function(name) \
  do {                            \
      } while (false)
#define SCOPE_profile_gpu_i(scopeName) \
  do {                                              \
      } while (false)
#define COUNT_profile_cpu(name, count) \
  do {                                 \
      } while (false)
#define COUNT_profile_gpu(name, count) \
  do {                                 \
      } while (false)

#endif
}