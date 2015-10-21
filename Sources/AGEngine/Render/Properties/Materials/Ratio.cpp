# include <Render/Properties/Materials/Ratio.hh>
# include <Render/ProgramResources/Types/Uniform/Vec1.hh>

namespace AGE
{
	Ratio::Ratio(std::string const &add_name) :
		AProperty(add_name + "_ratio"),
		_ratio(1.f)
	{

	}

	Ratio::Ratio(Ratio &&move) :
		AProperty(std::move(move)),
		_ratio(move._ratio)
	{

	}

	void Ratio::update(Vec1 *res, Ratio *ratio)
	{
		*res = ratio->_ratio;
	}

	void Ratio::instanciedUpdate(Vec1 *, Ratio *)
	{
		assert(false && "implement me");
	}

	float Ratio::get()
	{
		return (_ratio);
	}

	Ratio & Ratio::set(float ratio)
	{
		_ratio = ratio;
		return (*this);
	}
}