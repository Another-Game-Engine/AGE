#pragma once

#include <Systems/System.h>
#include <Core/EntityFilter.hpp>

namespace AGE
{
	class RenderCameraSystem : public System
	{
	public:
		RenderCameraSystem() = delete;
		RenderCameraSystem(AScene *scene);
		~RenderCameraSystem() = default;
		void drawDebugLines(bool activated);

	private:
		EntityFilter _cameras;
		EntityFilter _spotLights;
		EntityFilter _directionnalLights;
		EntityFilter _pointLights;
		bool         _drawDebugLines;

		virtual bool initialize();
		virtual void updateBegin(float time);
		virtual void mainUpdate(float time);
		virtual void updateEnd(float time);
	};
}
