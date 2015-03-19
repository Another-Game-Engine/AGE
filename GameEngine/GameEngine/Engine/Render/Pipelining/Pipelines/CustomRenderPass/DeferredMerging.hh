#pragma once

#include <Render/Pipelining/Render/ScreenRender.hh>

namespace AGE
{
	class Texture2D;
	class Program;

	class DeferredMerging : public ScreenRender
	{
	public:
		DeferredMerging(std::shared_ptr<PaintingManager> painterManager,
							std::shared_ptr<Texture2D> diffuse,
							std::shared_ptr<Texture2D> specular,
							std::shared_ptr<Texture2D> lightAccumulation);
		virtual ~DeferredMerging() = default;

	protected:
		virtual void renderPass(RenderPipeline const &, RenderLightList const &, CameraInfos const &);

	private:
		std::shared_ptr<Texture2D> _diffuseInput;
		std::shared_ptr<Texture2D> _specularInput;
		std::shared_ptr<Texture2D> _lightAccuInput;

		Key<Vertices> _quadVertices;
		std::shared_ptr<Painter> _quadPainter;
	};
}

