#pragma once

#include <Render/Pipelining/Render/ScreenRender.hh>

#include <glm/glm.hpp>

namespace AGE
{
	class Texture2D;
	class Program;

	class DeferredOnScreen : public ScreenRender
	{
	public:
		DeferredOnScreen(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
			std::shared_ptr<Texture2D> diffuse);
		virtual ~DeferredOnScreen() = default;

	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);

	private:
		std::shared_ptr<Texture2D> _diffuseInput;

		Key<Vertices> _quadVertices;
		std::shared_ptr<Painter> _quadPainter;

	};
}
