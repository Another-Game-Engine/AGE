
#include <TMQ/queue.hpp>
#include <Threads/ThreadManager.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Utils/Profiler.hpp>

#define LMT_ALLOC_NUMBER_PER_CHUNK 1024
#define LMT_STACK_SIZE_PER_ALLOC 25
#define LMT_CHUNK_NUMBER_PER_THREAD 4
#define LMT_CACHE_SIZE 32
#define LMT_ALLOC_DICTIONARY_SIZE 1024 * 8
#define LMT_STACK_DICTIONARY_SIZE 1024 * 8
#define LMT_TREE_DICTIONARY_SIZE 1024 * 32
#define LMT_IMGUI 1
#define LMT_IMGUI_INCLUDE_PATH "imgui/imgui.h"

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

	void free(void *ptr)
	{
		return LMT_DEALLOC(ptr);
	}

	void *realloc(void *ptr, size_t size)
	{
		return LMT_REALLOC(ptr, size);
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