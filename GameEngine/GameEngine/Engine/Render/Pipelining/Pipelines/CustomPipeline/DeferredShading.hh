#pragma once

#include <Render/Pipelining/Pipelines/ARenderingPipeline.hh>
#include <glm/glm.hpp>

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
		DeferredShading(glm::uvec2 const &screen_size, std::shared_ptr<PaintingManager> const &painter_manager);
		DeferredShading(DeferredShading &&move);
		virtual ~DeferredShading() {};
		DeferredShading(DeferredShading const &) = delete;
		DeferredShading &operator=(DeferredShading const &) = delete;

	public:
		virtual IRenderingPipeline &render(ARGS_FUNCTION_RENDER) override final;

	};
}