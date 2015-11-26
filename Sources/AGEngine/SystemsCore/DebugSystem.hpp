#pragma once

#include <System/System.h>
#include <Core/EntityFilter.hpp>

namespace AGE
{
	class DebugSystem : public System<DebugSystem>
	{
	public:
		DebugSystem() = delete;
		DebugSystem(AScene *scene);
		~DebugSystem() = default;

	private:
		EntityFilter _pointLights;
		float _timeMultiplier;
		int _windowWidth;
		int _windowHeight;
		bool _fullscreen;
		size_t _frameCap;

		virtual bool initialize();
		virtual void updateBegin(float time);
		virtual void mainUpdate(float time);
		virtual void updateEnd(float time);
	};
}
