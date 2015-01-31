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
		DeferredShading(glm::vec2 const &screen_size);
		DeferredShading(DeferredShading &&move);
		virtual ~DeferredShading() {}

	public:
		virtual IRenderingPipeline &render(ARGS_FUNCTION_RENDER) override final;

	private:
	};

}