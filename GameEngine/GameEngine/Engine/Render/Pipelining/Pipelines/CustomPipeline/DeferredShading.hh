#pragma once

# include <Render/Pipelining/Pipelines/ARenderingPipeline.hh>

namespace AGE
{

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
		DeferredShading(glm::vec2 const &screen_size, std::shared_ptr<PaintingManager> const &painter_manager);
		DeferredShading(DeferredShading &&move);
		virtual ~DeferredShading() = default;

	public:
		virtual IRenderingPipeline &render(ARGS_FUNCTION_RENDER) override final;

	};
}