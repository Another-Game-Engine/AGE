#pragma once

# include <Render/Pipelining/Render/ARendering.hh>

class Rendering : public ARendering
{
public:
	Rendering(std::function<void (FUNCTION_ARGS)> const &function);
	Rendering(Rendering &&move);
	virtual ~Rendering() {}

public:
	virtual IRendering &render(FUNCTION_ARGS) override final;
};