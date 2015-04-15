#include "DepthMapManager.hpp"
#include "DepthMapHandle.hpp"
#include "DepthMap.hpp"
#include <mutex>

namespace AGE
{
	DepthMapManager::DepthMapManager()
	{
	}

	void DepthMapManager::init(std::size_t width, std::size_t height, std::size_t mipmalLevel)
	{
		_maps[0].init(width, height, mipmalLevel);
		_maps[1].init(width, height, mipmalLevel);
		_status[0] = Writable;
		_status[1] = Writable;
		_mipmapLevel = mipmalLevel;
	}

	DepthMapHandle DepthMapManager::getReadableMap()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		std::size_t index = 0;
		if (_status[index] != Readable)
		{
			index = 1;
			if (_status[index] != Readable)
			{
				index = -1;
			}
		}
		if (index == -1)
		{
			return DepthMapHandle(this, nullptr, index, Reading);
		}
		else
		{
			_status[index] = Reading;
			return DepthMapHandle(this, &_maps[index], index, Reading);
		}
	}

	DepthMapHandle DepthMapManager::getWritableMap()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		std::size_t index = 0;
		if (_status[index] == Reading)
		{
			index = 1;
			if (_status[index] != Reading)
			{
				index = -1;
			}
		}
		if (index == -1)
		{
			return DepthMapHandle(this, nullptr, index, Writing);
		}
		else
		{
			_status[index] = Writing;
			return DepthMapHandle(this, &_maps[index], index, Writing);
		}
	}
	void DepthMapManager::_releaseMap(std::size_t index)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		if (index >= 2)
		{
			return;
		}
		if (_status[index] == Reading)
		{
			_status[index] = Readable;
		}
		if (_status[index] == Writing)
		{
			_status[index] = Readable;
		}
	}
}