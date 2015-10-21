#include <Render/Properties/Materials/ScaleUVs.hpp>
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>

namespace AGE
{
	ScaleUVs::ScaleUVs() :
		AProperty("scaleUvs"),
		_tof(false)
	{

	}

	ScaleUVs::ScaleUVs(ScaleUVs &&move) :
		AProperty(std::move(move)),
		_tof(move._tof)
	{

	}

	void ScaleUVs::update(Vec1 *res, ScaleUVs *scale)
	{
		*res = scale->_tof ? 1.0f : 0.0f;
	}

	void ScaleUVs::instanciedUpdate(Vec1 *, ScaleUVs *)
	{
		assert(false && "implement me");
	}

	bool const & ScaleUVs::get()
	{
		return (_tof);
	}

	ScaleUVs & ScaleUVs::set(bool const &tof)
	{
		_tof = tof;
		return (*this);
	}

}