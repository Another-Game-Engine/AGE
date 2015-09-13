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

	void Ratio::_update(IProgramResources *p)
	{
		auto resource = (Vec1*)(p);
		*resource = _ratio;
	}

	float Ratio::get()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return (_ratio);
	}

	Ratio & Ratio::set(float ratio)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_ratio = ratio;
		return (*this);
	}
}