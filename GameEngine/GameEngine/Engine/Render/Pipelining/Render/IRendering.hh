#pragma once

# include <vector>
# include <memory>

class IRenderingPipeline;

# define FUNCTION_ARGS IRenderingPipeline &, std::vector<std::shared_ptr<IRenderingPipeline>> const &

class IRendering
{
public:
	virtual ~IRendering() {}
	virtual IRendering &render(FUNCTION_ARGS) = 0;
};