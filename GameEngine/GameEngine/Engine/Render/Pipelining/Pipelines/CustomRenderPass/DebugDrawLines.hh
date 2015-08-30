#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>

#include <glm/glm.hpp>

namespace AGE
{
	class Texture2D;
	class Program;

	class DebugDrawLines : public FrameBufferRender
	{
	public:
		DebugDrawLines(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
			std::shared_ptr<Texture2D> diffuse,
			std::shared_ptr<Texture2D> depth);
		virtual ~DebugDrawLines() = default;

	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);

	private:
		std::shared_ptr<Texture2D> _diffuseInput;
		std::shared_ptr<Texture2D> _lightAccuInput;
		std::shared_ptr<Texture2D> _shinyAccuInput;
	};
}