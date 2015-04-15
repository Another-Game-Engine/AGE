#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace AGE
{
	class DepthMapHandle;

	class DepthMap
	{
	public:
		void init(std::size_t width, std::size_t height, std::size_t mipmalLevel);
		bool passTest(float pixelDepth, std::size_t x, std::size_t y) const;
		const glm::mat4 &getMV() const;
		std::size_t getMipmapWidth() const { return _mipmapWidth; }
		std::size_t getMipmapHeight() const { return _mipmapHeight; }
	private:
		std::vector<uint32_t> _buffer;
		std::size_t _width;
		std::size_t _height;
		std::size_t _mimpapLevel;
		std::size_t _mipmapWidth;
		std::size_t _mipmapHeight;
		glm::mat4 _mv;
		friend class DepthMapHandle;
	};
}