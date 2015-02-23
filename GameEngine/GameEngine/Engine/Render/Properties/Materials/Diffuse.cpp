# include <Render/Properties/Materials/Diffuse.hh>


namespace AGE
{
	Diffuse::Diffuse() :
		AProperty(std::string("diffuse_")),
		_ratio("diffuse_"),
		_color("diffuse_"),
		_mapColor("diffuse_")
	{

	}

	Diffuse::Diffuse(Diffuse &&move) :
		AProperty(std::move(move)),
		_ratio(std::move(move._ratio)),
		_color(std::move(move._color)),
		_mapColor(std::move(move._mapColor))
	{

	}

	void Diffuse::_update(std::shared_ptr<Program> const &p)
	{
		_ratio.update(p);
		_color.update(p);
		_mapColor.update(p);
	}

	float Diffuse::get_ratio()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return (_ratio.get());
	}

	Diffuse &Diffuse::set_ratio(float ratio)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_ratio.set(ratio);
		return (*this);
	}

	glm::vec4 const &Diffuse::get_color()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return (_color.get());
	}

	Diffuse &Diffuse::set_color(glm::vec4 const &color)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_color.set(color);
		return (*this);
	}

	std::shared_ptr<Texture2D> const & Diffuse::get_map()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return (_mapColor.get());
	}

	Diffuse & Diffuse::set_map(std::shared_ptr<Texture2D> const &m)
	{
		_mapColor.set(m);
		return (*this);
	}

}
