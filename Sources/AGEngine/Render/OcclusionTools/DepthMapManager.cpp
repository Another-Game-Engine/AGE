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
		_status[0] = Writing;
		_status[1] = Writing;
		_mipmapLevel = mipmalLevel;
		_lastReadable = -1;
	}

	DepthMapHandle DepthMapManager::getReadableMap()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		if (_lastReadable > 1)
		{
			return DepthMapHandle(this, nullptr, -1, Reading);
		}
		else
		{
			_status[_lastReadable] = Reading;
			return DepthMapHandle(this, &_maps[_lastReadable], _lastReadable, Reading);
		}
	}

	DepthMapHandle DepthMapManager::getWritableMap()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);

		std::size_t index = 0;
		if (_status[index] == Reading)
		{
			index = 1;
			if (_status[index] == Reading)
			{
				index = -1;
			}
		}
		if (index > 1)
		{
			return DepthMapHandle(this, nullptr, index, Writing);
		}
		else
		{
			if (_lastReadable == index)
			{
				_lastReadable = -1;
			}
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
			_lastReadable = index;
		}
	}
}