#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>

#include <glm/glm.hpp>

namespace AGE
{
	class Texture2D;
	class Program;

	class DeferredMergingOffScreen : public FrameBufferRender
	{
	public:
		DeferredMergingOffScreen(std::shared_ptr<PaintingManager> painterManager,
			std::shared_ptr<Texture2D> diffuse,
			std::shared_ptr<Texture2D> specular,
			std::shared_ptr<Texture2D> lightAccumulation);
		virtual ~DeferredMergingOffScreen() = default;

		void setAmbient(glm::vec3 const &ambient);

	protected:
		virtual void renderPass(RenderPipeline const &, RenderLightList const &, CameraInfos const &);

	private:
		std::shared_ptr<Texture2D> _diffuseInput;
		std::shared_ptr<Texture2D> _specularInput;
		std::shared_ptr<Texture2D> _lightAccuInput;

		Key<Vertices> _quadVertices;
		std::shared_ptr<Painter> _quadPainter;

		glm::vec3 _ambientColor;
	};
}
