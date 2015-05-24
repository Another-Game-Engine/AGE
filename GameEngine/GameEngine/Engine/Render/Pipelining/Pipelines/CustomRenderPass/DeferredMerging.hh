#pragma once

#include <Render/Pipelining/Render/ScreenRender.hh>

#include <glm/glm.hpp>

namespace AGE
{
	class Texture2D;
	class Program;

	class DeferredMerging : public ScreenRender
	{
	public:
		DeferredMerging(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
							std::shared_ptr<Texture2D> diffuse,
							std::shared_ptr<Texture2D> lightAccumulation,
							std::shared_ptr<Texture2D> shinyAccumulation);
		virtual ~DeferredMerging() = default;

		void setAmbient(glm::vec3 const &ambient);

	protected:
		virtual void renderPass(RenderPipeline const &, RenderLightList &, CameraInfos const &);

	private:
		std::shared_ptr<Texture2D> _diffuseInput;
		std::shared_ptr<Texture2D> _lightAccuInput;
		std::shared_ptr<Texture2D> _shinyAccuInput;

		Key<Vertices> _quadVertices;
		std::shared_ptr<Painter> _quadPainter;

		glm::vec3 _ambientColor;
	};
}

