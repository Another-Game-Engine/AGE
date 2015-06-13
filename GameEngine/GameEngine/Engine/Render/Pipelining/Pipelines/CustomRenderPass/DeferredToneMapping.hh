#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>

#include <glm/glm.hpp>

#define MAX_TONE_MAPPING 6.0f
#define MIN_TONE_MAPPING 0.01f
#define SPEED_TONE_MAPPING 0.1f
#define MIDDLE_GRAY_TONE_MAPPING 0.5f

namespace AGE
{
	class Texture2D;
	class Program;

	class DeferredToneMapping : public FrameBufferRender
	{
	public:
		DeferredToneMapping(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
			std::shared_ptr<Texture2D> diffuse);
		virtual ~DeferredToneMapping() = default;

		void setAmbient(glm::vec3 const &ambient);

	protected:
		virtual void renderPass(RenderPipeline const &, RenderLightList &, CameraInfos const &);

	private:
		std::shared_ptr<Texture2D> _diffuseInput;

		float _targetLuminance;
		float _currentLuminance;
		Key<Vertices> _quadVertices;
		std::shared_ptr<Painter> _quadPainter;
	};
}
