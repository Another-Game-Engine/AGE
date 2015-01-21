# include <Render/Properties/Materials/Diffuse.hh>

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

IProperty & Diffuse::update(std::shared_ptr<Program> const &p)
{
	_ratio.update(p);
	_color.update(p);
	_mapColor.update(p);
	return (*this);
}

float Diffuse::get_ratio() const
{
	return (_ratio.get());
}

Diffuse &Diffuse::set_ratio(float ratio)
{
	_ratio.set(ratio);
	return (*this);
}

glm::vec4 const &Diffuse::get_color()
{
	return (_color.get());
}

Diffuse &Diffuse::set_color(glm::vec4 const &color)
{
	_color.set(color);
	return (*this);
}

std::shared_ptr<Texture2D> const & Diffuse::get_map() const
{
	return (_mapColor.get());
}

Diffuse & Diffuse::set_map(std::shared_ptr<Texture2D> const &m)
{
	_mapColor.set(m);
	return (*this);
}

IProperty & Diffuse::set_program(std::vector<std::shared_ptr<Program>> const &programs)
{
	AProperty::set_program(programs);
	_ratio.set_program(programs);
	_color.set_program(programs);
	_mapColor.set_program(programs);
	return (*this);
}

