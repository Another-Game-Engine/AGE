#pragma once

#include <Render/Pipelining/Pipelines/ARenderingPipeline.hh>

namespace AGE
{
	class BasicPipeline : public ARenderingPipeline
	{
	public:
		enum Steps
		{
			RENDER_SKINNED = 0,
			RENDER_BASIC,
			TOTAL
		};

	public:
		BasicPipeline(std::shared_ptr<PaintingManager> const &painter_manager);
		BasicPipeline(BasicPipeline &&move);
		virtual ~BasicPipeline() = default;

	public:
		virtual IRenderingPipeline &render(ARGS_FUNCTION_RENDER) override final;

	};
}