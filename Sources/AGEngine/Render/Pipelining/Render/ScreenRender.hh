#pragma once

# include <Render/Pipelining/Render/ARender.hh>
#include <glm/glm.hpp>

namespace AGE
{
	struct DRBCameraDrawableList;

	class ScreenRender : public ARender
	{
	public:
		ScreenRender(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager);
		ScreenRender(ScreenRender &&move);
		virtual ~ScreenRender() {}

	public:
		virtual IRender &render(const DRBCameraDrawableList &infos) override final;
	
	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos) = 0;
		glm::uvec2 viewport;
	};
}