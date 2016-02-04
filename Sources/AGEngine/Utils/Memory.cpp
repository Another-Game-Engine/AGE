
#include <TMQ/queue.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Utils/Profiler.hpp>

#define LMT_ALLOC_NUMBER_PER_CHUNK 512
#define LMT_STACK_SIZE_PER_ALLOC 50
#define LMT_CHUNK_NUMBER_PER_THREAD 6
#define LMT_CACHE_SIZE 8
#define LMT_ALLOC_DICTIONARY_SIZE 1024 * 16
#define LMT_STACK_DICTIONARY_SIZE 1024 * 16
#define LMT_TREE_DICTIONARY_SIZE 1024 * 32
#define LMT_IMGUI 1
#define LMT_PLATFORM_WINDOWS 1
#define LMT_IMGUI_INCLUDE_PATH "imgui/imgui.h"
#define LMT_USE_MALLOC ::malloc
#define LMT_USE_REALLOC ::realloc
#define LMT_USE_FREE ::free
#define LMT_STATS 1

#ifdef AGE_DEBUG
#define LMT_DEBUG_DEV 1
#endif

#define LMT_TREAT_CHUNK(chunk) \
SCOPE_profile_cpu_i("Alloc", "Alloc chunk"); \
TMQ::TaskManager::emplaceSharedTask<AGE::Tasks::Basic::VoidFunction>([=](){LiveMemTracer::treatChunk(chunk);}); \

#define LMT_IMPL 1

#include "memory.hpp"

namespace AGE
{
	void *malloc(size_t size)
	{
		return LMT_ALLOC(size);
	}

	void *malloc_aligned(size_t size, size_t alignment)
	{
		return LMT_ALLOC_ALIGNED(size, alignment);
	}

	void free(void *ptr)
	{
		return LMT_DEALLOC(ptr);
	}

	void free_aligned(void *ptr)
	{
		return LMT_DEALLOC_ALIGNED(ptr);
	}

	void *realloc(void *ptr, size_t size)
	{
		return LMT_REALLOC(ptr, size);
	}

	void *realloc_aligned(void *ptr, size_t size, size_t alignment)
	{
		return LMT_REALLOC_ALIGNED(ptr, size, alignment);
	}
}

#define OVERRIDE_NEW 1

#ifdef OVERRIDE_NEW
//////////////////////////////////////////////////////////////////////////
void* operator new(size_t count) throw(std::bad_alloc)
{
	return AGE_MALLOC(count);
}

void* operator new(size_t count, const std::nothrow_t&) throw()
{
	return AGE_MALLOC(count);
}

void* operator new(size_t count, size_t alignment) throw(std::bad_alloc)
{
	return AGE_MALLOC_ALIGNED(count, alignment);
}

void* operator new(size_t count, size_t alignment, const std::nothrow_t&) throw()
{
	return AGE_MALLOC_ALIGNED(count, alignment);
}

void* operator new[](size_t count) throw(std::bad_alloc)
{
	return AGE_MALLOC(count);
}

void* operator new[](size_t count, const std::nothrow_t&) throw()
{
	return AGE_MALLOC(count);
}

void* operator new[](size_t count, size_t alignment) throw(std::bad_alloc)
{
	return AGE_MALLOC_ALIGNED(count, alignment);
}

void* operator new[](size_t count, size_t alignment, const std::nothrow_t&) throw()
{
	return AGE_MALLOC_ALIGNED(count, alignment);
}

void operator delete(void* ptr) throw()
{
	return AGE_FREE(ptr);
}

void operator delete(void *ptr, const std::nothrow_t&) throw()
{
	return AGE_FREE(ptr);
}

void operator delete(void *ptr, size_t alignment) throw()
{
	return AGE_FREE_ALIGNED(ptr);
}

void operator delete(void *ptr, size_t alignment, const std::nothrow_t&) throw()
{
	return AGE_FREE_ALIGNED(ptr);
}

void operator delete[](void* ptr) throw()
{
	return AGE_FREE(ptr);
}

void operator delete[](void *ptr, const std::nothrow_t&) throw()
{
	return AGE_FREE(ptr);
}

void operator delete[](void *ptr, size_t alignment) throw()
{
	return AGE_FREE_ALIGNED(ptr);
}

void operator delete[](void *ptr, size_t alignment, const std::nothrow_t&) throw()
{
	return AGE_FREE_ALIGNED(ptr);
}
//////////////////////////////////////////////////////////////////////////
#endif