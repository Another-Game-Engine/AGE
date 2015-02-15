#pragma once

# include <Render/Pipelining/Render/IRendering.hh>
# include <functional>

namespace AGE
{

	class ARendering : public IRendering
	{
	public:
		virtual ~ARendering() {}

	protected:
		ARendering(std::function<void(FUNCTION_ARGS)> const & function);
		ARendering(ARendering &&move);

	protected:
		std::function<void(FUNCTION_ARGS)> _render_function;
	};

}