# include <Render/Properties/Materials/Specular.hh>


namespace AGE
{
	Specular::Specular() :
		AProperty(std::string("specular_")),
		_ratio("specular_"),
		_color("specular_"),
		_mapColor("specular_")
	{

	}

	Specular::Specular(Specular &&move) :
		AProperty(std::move(move)),
		_ratio(std::move(move._ratio)),
		_color(std::move(move._color)),
		_mapColor(std::move(move._mapColor))
	{

	}

	void Specular::_update(IProgramResources *p)
	{
		_ratio.update(p);
		_color.update(p);
		_mapColor.update(p);
	}

	float Specular::get_ratio()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return (_ratio.get());
	}

	Specular &Specular::set_ratio(float ratio)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_ratio.set(ratio);
		return (*this);
	}

	glm::vec4 const &Specular::get_color()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return (_color.get());
	}

	Specular &Specular::set_color(glm::vec4 const &color)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_color.set(color);
		return (*this);
	}

	std::shared_ptr<Texture2D> const & Specular::get_map()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return (_mapColor.get());
	}

	Specular & Specular::set_map(std::shared_ptr<Texture2D> const &m)
	{
		_mapColor.set(m);
		return (*this);
	}

}