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
		virtual void _update(std::shared_ptr<Program> const &program) override final;

	public:
		bool const &get();
		ScaleUVs &set(bool const &tof);

	private:
		bool _tof;
	};
}