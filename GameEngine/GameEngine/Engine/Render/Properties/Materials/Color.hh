#pragma once

# include <Render/Properties/AProperty.hh>

namespace AGE
{
	class Vec4;

	class Color : public AProperty
	{
	public:
		Color(std::string const &add_name);
		Color(Color &&move);
		PROPERTY_UPDATE_FUNCTION(Color, Vec4);
		PROPERTY_INSTANCIED_UPDATE_FUNCTION(Color, Vec4);

	public:
		glm::vec4 const &get();
		Color &set(glm::vec4 const &color);

	private:
		glm::vec4 _color;
	};
}
