#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <glm\glm.hpp>

namespace AGE
{
	class Texture2D;

	class DeferredSkyBox : public FrameBufferRender
	{
	public:
		DeferredSkyBox(glm::uvec2 const &screenSize,
			std::shared_ptr<PaintingManager> painterManager,
			std::shared_ptr<Texture2D> diffuse,
			std::shared_ptr<Texture2D> depth,
			std::shared_ptr<Texture2D> lightAccumulation);
		virtual ~DeferredSkyBox() = default;

		void setSkyboxLighting(glm::vec3 lighting);

	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);
		Key<Painter> _painterCube;
		Key<Vertices> _cube;

		glm::vec3 _lighting;
	};
}
