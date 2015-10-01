#pragma once

# include <vector>
# include <memory>
# include <Render/Program.hh>
# include <Render/Properties/AProperty.hh>
# include <glm/glm.hpp>

namespace AGE
{
	class Mat4;
	class SamplerBuffer;
	class Mat4Array255;

	class SkeletonProperty : public AProperty
	{
	public:
		SkeletonProperty();
		virtual ~SkeletonProperty();
		SkeletonProperty(SkeletonProperty &&move);
		SkeletonProperty(SkeletonProperty const &copy) = delete;
		SkeletonProperty &operator=(SkeletonProperty const &other) = delete;
		PROPERTY_UPDATE_FUNCTION(SkeletonProperty, Mat4Array255);
		PROPERTY_INSTANCIED_UPDATE_FUNCTION(SkeletonProperty, Mat4Array255);
	public:
		glm::mat4 *get();
		SkeletonProperty &set(const std::vector<glm::mat4> &mat);

	private:
		glm::mat4 _matrixArray[255];
		std::size_t _size = 0;
	};
}
