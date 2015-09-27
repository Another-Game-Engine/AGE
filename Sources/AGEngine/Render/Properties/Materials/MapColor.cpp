# include <Render/Properties/Materials/MapColor.hh>
# include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>

namespace AGE
{
	MapColor::MapColor(std::string const &add_name) :
		AProperty(add_name + "_map")
	{

	}

	MapColor::MapColor(MapColor &&move) :
		AProperty(std::move(move)),
		_texture(std::move(move._texture))
	{

	}

	void MapColor::update(Sampler2D *res, MapColor *color)
	{
		*res = color->_texture;
	}

	void MapColor::instanciedUpdate(Sampler2D *, MapColor *)
	{
		assert(false && "implement me");
	}

	std::shared_ptr<Texture2D> const & MapColor::get()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return (_texture);
	}

	MapColor & MapColor::set(std::shared_ptr<Texture2D> const &map)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_texture = map;
		return (*this);
	}
}
