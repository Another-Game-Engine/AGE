#pragma once

#include "DepthMap.hpp"
#include <Utils/SpinLock.hpp>

namespace AGE
{
	class DepthMapHandle;

	class DepthMapManager
	{
	public:
		enum Status
		{
			Readable = 0,
			Writable = 1,
			Reading = 2,
			Writing = 3
		};

		DepthMapManager();
		void init(std::size_t width, std::size_t height, std::size_t mipmalLevel);
		DepthMapHandle getReadableMap();
		DepthMapHandle getWritableMap();
		inline std::size_t getMipmapLevel() const { return _mipmapLevel; }
	private:
		void _releaseMap(std::size_t index);
		DepthMap _maps[2];
		uint32_t _status[2];
		AGE::SpinLock _mutex;
		std::size_t _mipmapLevel;

		friend class DepthMapHandle;
	};
}