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

	private:
		EntityFilter _cameras;

		virtual bool initialize();
		virtual void updateBegin(float time);
		virtual void mainUpdate(float time);
		virtual void updateEnd(float time);
	};
}
