# include <Render/Properties/Materials/Ratio.hh>
# include <Render/ProgramResources/Types/Uniform/Vec1.hh>

namespace AGE
{
	Ratio::Ratio(std::string const &add_name) :
		AProperty(add_name + "ratio"),
		_ratio(1.f)
	{

	}

	Ratio::Ratio(Ratio &&move) :
		AProperty(std::move(move)),
		_ratio(move._ratio)
	{

	}

	IProperty & Ratio::update(std::shared_ptr<Program> const &p)
	{
		auto &resource = *std::static_pointer_cast<Vec1>(get_resource(p));
		resource = _ratio;
		return (*this);
	}

	float Ratio::get() const
	{
		return (_ratio);
	}

	Ratio & Ratio::set(float ratio)
	{
		_ratio = ratio;
		return (*this);
	}
}