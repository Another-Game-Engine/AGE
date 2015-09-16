#pragma once

# include <Render/Properties/AProperty.hh>

namespace AGE
{
	class Texture2D;
	class Sampler2D;

	class MapColor : public AProperty
	{
	public:
		MapColor(std::string const &add_name);
		MapColor(MapColor &&move);
		PROPERTY_UPDATE_FUNCTION(MapColor, Sampler2D);
		PROPERTY_INSTANCIED_UPDATE_FUNCTION(MapColor, Sampler2D);
	public:
		std::shared_ptr<Texture2D> const &get();
		MapColor &set(std::shared_ptr<Texture2D> const &map);
		
		typedef std::shared_ptr<Texture2D> type_t;
	private:
		std::shared_ptr<Texture2D> _texture;
	};
}
