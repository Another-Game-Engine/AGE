#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <glm\glm.hpp>

namespace AGE
{
	class Texture2D;

	class DeferredBlur : public FrameBufferRender
	{
	public:
		DeferredBlur(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
							std::shared_ptr<Texture2D> diffuse,
                            std::shared_ptr<Texture2D> blur);
		virtual ~DeferredBlur() = default;

	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);
        std::shared_ptr<Texture2D> _diffuseInput;
        std::shared_ptr<Texture2D> _blurInput;
        Key<Vertices> _quadVertices;
		std::shared_ptr<Painter> _quadPainter;
	};
}
