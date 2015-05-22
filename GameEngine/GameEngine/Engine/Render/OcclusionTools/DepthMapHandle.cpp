#include "DepthMap.hpp"
#include "DepthMapHandle.hpp"
#include "DepthMapManager.hpp"
#include <Utils/Debug.hpp>

namespace AGE
{
	DepthMapHandle::~DepthMapHandle()
	{
		AGE_ASSERT(_manager != nullptr);
		_manager->_releaseMap(_index);
	}

	const DepthMap *DepthMapHandle::operator->()
	{
		return _map;
	}

	bool DepthMapHandle::isValid() const
	{
		return _map != nullptr;
	}

	bool DepthMapHandle::isWritable() const
	{
		return _status == DepthMapManager::Status::Writing;
	}

	std::vector<uint32_t> &DepthMapHandle::getWritableBuffer()
	{
		AGE_ASSERT(isWritable());
		return _map->_buffer;
	}

	void DepthMapHandle::setMV(const glm::mat4 &mv)
	{
		AGE_ASSERT(isWritable());
		_map->_mv = mv;
	}

	DepthMapHandle::DepthMapHandle(DepthMapManager *managerPtr, DepthMap *map, std::size_t index, DepthMapManager::Status status)
		: _manager(managerPtr)
		, _map(map)
		, _index(index)
		, _status(status)
	{
		AGE_ASSERT(_manager != nullptr);
	}
}