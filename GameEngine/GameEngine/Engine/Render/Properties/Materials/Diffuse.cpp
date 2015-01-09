# include <Render/Properties/Materials/Diffuse.hh>
# include <Render/Program.hh>

Diffuse::Diffuse(std::vector<std::shared_ptr<Program>> const &programs) :
AProperty(std::string("diffuse_")),
_ratio("diffuse_", programs)
{

}

Diffuse::Diffuse(Diffuse &&move) :
AProperty(std::move(move)),
_ratio(move._ratio)
{

}

IProperty & Diffuse::update(std::shared_ptr<Program> const &p)
{
	_ratio.update(p);
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



