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

	void ScaleUVs::_update(IProgramResources *program)
	{
		auto resource = (Vec1*)(program);
		*resource = _tof ? 1.0f : 0.0f;
	}

	bool const & ScaleUVs::get()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return (_tof);
	}

	ScaleUVs & ScaleUVs::set(bool const &tof)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_tof = tof;
		return (*this);
	}

}