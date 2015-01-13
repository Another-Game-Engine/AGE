#pragma once

# include <Render/Pipelining/Render/IRendering.hh>
# include <functional>

class ARendering : public IRendering
{
public:
	virtual ~ARendering() {}

protected:
	ARendering(std::function<IRendering &(FUNCTION_ARGS)> const & function);
	ARendering(ARendering &&move);

protected:
	std::function<IRendering &(FUNCTION_ARGS)> _render_function;
};