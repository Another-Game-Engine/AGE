#pragma once

# include <Render/Properties/AProperty.hh>

namespace AGE
{
	class ScaleUVs : public AProperty
	{
	public:
		ScaleUVs();
		ScaleUVs(ScaleUVs &&move);

	private:
		virtual void _update(IProgramResources *program) override final;

	public:
		bool const &get();
		ScaleUVs &set(bool const &tof);

	private:
		bool _tof;
	};
}