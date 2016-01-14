#pragma once

#include "../../LiveMemTrace/src/LiveMemTracer.hpp"

namespace AGE
{
	void *malloc(size_t size);
	void *malloc_aligned(size_t size, size_t alignment);
	void free(void *ptr);
	void free_aligned(void *ptr);
	void *realloc(void *ptr, size_t size);
	void *realloc_aligned(void *ptr, size_t size, size_t alignment);
}

#define AGE_MALLOC(size) AGE::malloc(size)
#define AGE_FREE(ptr) AGE::free(ptr)
#define AGE_MALLOC_ALIGNED(size, alignment) AGE::malloc_aligned(size, alignment)
#define AGE_FREE_ALIGNED(ptr) AGE::free_aligned(ptr)
#define AGE_REALLOC(ptr, size) AGE::realloc(ptr, size)
#define AGE_REALLOC_ALIGNED(ptr, size, alignment) AGE::realloc_aligned(ptr, size, alignment)