#pragma once

# include <Render/Properties/AProperty.hh>

namespace AGE
{
	class Vec1;

	class ScaleUVs : public AProperty
	{
	public:
		ScaleUVs();
		ScaleUVs(ScaleUVs &&move);
		PROPERTY_UPDATE_FUNCTION(ScaleUVs, Vec1);
		PROPERTY_INSTANCIED_UPDATE_FUNCTION(ScaleUVs, Vec1);

	public:
		bool const &get();
		ScaleUVs &set(bool const &tof);

	private:
		bool _tof;
	};
}