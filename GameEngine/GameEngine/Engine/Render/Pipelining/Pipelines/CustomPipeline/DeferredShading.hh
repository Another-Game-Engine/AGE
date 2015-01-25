#pragma once

# include <Render/Pipelining/Pipelines/ARenderingPipeline.hh>

class DeferredShading : public ARenderingPipeline
{
public:
	enum Step
	{
		BUFFERING = 0,
		LIGHTNING,
		MERGING,
		TOTAL
	};

public:
	DeferredShading(glm::mat4 const &perspective, glm::vec2 const &screen_size);
	DeferredShading(DeferredShading &&move);
	virtual ~DeferredShading() {}

public:
	virtual IRenderingPipeline &render(std::vector<AGE::Drawable> const &to_render, PaintingManager const &paintingManager) override final;

private:
};