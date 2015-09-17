#pragma once

#include <Systems/System.h>

namespace AGE
{
	class LifetimeSystem : public AGE::System
	{
	public:
		LifetimeSystem(AScene *scene);
		virtual ~LifetimeSystem();
	private:
		EntityFilter _filter;
		virtual void updateBegin(float time);
		virtual void updateEnd(float time);
		virtual void mainUpdate(float time);
		virtual bool initialize();
	};
}