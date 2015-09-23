#pragma once

#include <System/System.h>
#include <Core/EntityFilter.hpp>

namespace AGE
{
	class BasicDemoCameraSystem : public System<BasicDemoCameraSystem>
	{
	public:
		BasicDemoCameraSystem() = delete;
		BasicDemoCameraSystem(AScene *scene);
		~BasicDemoCameraSystem() = default;

	private:
		virtual bool initialize();
		virtual void updateBegin(float time);
		virtual void mainUpdate(float time);
		virtual void updateEnd(float time);

		EntityFilter _filter;
		int _pipelineIndex = 0;
	};
}

