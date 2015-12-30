#pragma once

#include "../../LiveMemTrace/src/LiveMemTracer.hpp"

namespace AGE
{
	void *malloc(size_t size);
	void free(void *ptr);
	void *realloc(void *ptr, size_t size);
}

#define AGE_MALLOC(size) AGE::malloc(size)
#define AGE_FREE(ptr) AGE::free(ptr)
#define AGE_MALLOC_ALIGNED(size, alignment) AGE::malloc(size)
#define AGE_FREE_ALIGNED(ptr) AGE::free(ptr)
#define AGE_REALLOC(ptr, size) AGE::realloc(ptr, size)