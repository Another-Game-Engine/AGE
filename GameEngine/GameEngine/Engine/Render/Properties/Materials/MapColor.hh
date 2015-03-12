#pragma once

# include <Render/Properties/AProperty.hh>

namespace AGE
{
	class Texture2D;

	class MapColor : public AProperty
	{
	public:
		MapColor(std::string const &add_name);
		MapColor(MapColor &&move);

	private:
		virtual void _update(std::shared_ptr<Program> const &p) override;

	public:
		std::shared_ptr<Texture2D> const &get();
		MapColor &set(std::shared_ptr<Texture2D> const &map);
		
		typedef std::shared_ptr<Texture2D> type_t;
	private:
		std::shared_ptr<Texture2D> _texture;
	};
}
