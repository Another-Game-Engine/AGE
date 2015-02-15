#pragma once

#include <Render/Pipelining/Pipelines/ARenderingPipeline.hh>
#include <glm/glm.hpp>

namespace AGE
{

	class Vertices;

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

	private:
		static std::vector<glm::vec2> _quadPos;

		Key<Painter> _quadPainter;
		Key<Vertices> _quadVertices;
	};
}