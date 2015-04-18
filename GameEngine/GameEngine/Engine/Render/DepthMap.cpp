#include "DepthMap.hpp"
#include "DepthMapHandle.hpp"

namespace AGE
{
	void DepthMap::init(std::size_t width, std::size_t height, std::size_t mipmalLevel)
	{
		_width = width;
		_height = height;
		_mimpapLevel = mipmalLevel;

		_mipmapWidth = _width;
		_mipmapHeight = _height;

		std::size_t mipMapCounter = _mimpapLevel;
		while (mipMapCounter > 0)
		{
			_mipmapWidth /= 2;
			_mipmapHeight /= 2;
			--mipMapCounter;
		}
		_buffer.resize(_mipmapHeight * _mipmapWidth, -1);
	}

	bool DepthMap::passTest(uint32_t pixelDepth, std::size_t x, std::size_t y) const
	{
		if (x >= _width)
		{
			return false;
		}
		if (y >= _height)
		{
			return false;
		}

		std::size_t index = x + y * _mipmapWidth;

		if (index >= _buffer.size())
		{
			return false;
		}
		
		uint32_t depth = (_buffer[index] & 0xFFFFFF00) >> 8;
		if (depth < pixelDepth)
		{
			return false;
		}
		return true;
	}

	const glm::mat4 &DepthMap::getMV() const { return _mv; }
}