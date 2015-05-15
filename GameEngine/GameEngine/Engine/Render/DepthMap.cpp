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

	bool DepthMap::testPixel(uint32_t pixelDepth, std::size_t x, std::size_t y) const
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

	bool DepthMap::testBox(uint32_t pixelDepth, glm::uvec2 min, glm::uvec2 max) const
	{
		for (std::size_t y = min.y; y < max.y; ++y)
		{
			for (std::size_t x = min.x; x < max.x; ++x)
			{
				std::size_t index = x + y * _mipmapWidth;
				uint32_t depth = (_buffer[index] & 0xFFFFFF00) >> 8;
				if (depth >= pixelDepth) //				if ((int64_t)((int64_t)depth - pixelDepth) > 0)
				{
					return true;
				}
			}
		}
		return false;
	}

	glm::mat4 DepthMap::getMV() const { return _mv; }
}