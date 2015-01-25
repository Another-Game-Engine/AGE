#include <Render/Pipelining/Render/ARendering.hh>

ARendering::ARendering(std::function<void (FUNCTION_ARGS)> const &function) :
_render_function(function)
{

}

ARendering::ARendering(ARendering &&move) :
_render_function(std::move(move._render_function))
{

}

