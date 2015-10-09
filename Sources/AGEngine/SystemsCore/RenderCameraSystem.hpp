#pragma once

#include <System/System.h>
#include <Core/EntityFilter.hpp>

namespace AGE
{
	class RenderCameraSystem : public System<RenderCameraSystem>
	{
	public:
		RenderCameraSystem() = delete;
		RenderCameraSystem(AScene *scene);
		~RenderCameraSystem() = default;
		void drawDebugLines(bool activated);
		inline bool &enableCulling() { return _cullingEnabled; }
	private:
		EntityFilter _cameras;
		EntityFilter _spotLights;
		EntityFilter _directionnalLights;
		EntityFilter _pointLights;
		bool         _drawDebugLines;
		bool         _cullingEnabled;

		virtual bool initialize();
		virtual void updateBegin(float time);
		virtual void mainUpdate(float time);
		virtual void updateEnd(float time);
	};
}
