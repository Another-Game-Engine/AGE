#include "ObjectPool.hpp"

namespace AGE
{
	ObjectPoolBase::ObjectPoolBase(std::size_t chunkSize)
		: _chunkSize(chunkSize)
	{

	}

	ObjectPoolBase::~ObjectPoolBase()
	{
	}
}