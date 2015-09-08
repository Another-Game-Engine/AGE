#pragma once

#include <Systems/System.h>
#include <Core/EntityFilter.hpp>

namespace AGE
{
	class CharacterControllerSystem : public System
	{
	public:
		CharacterControllerSystem() = delete;
		CharacterControllerSystem(AScene *scene);
		~CharacterControllerSystem() = default;

	private:
		virtual bool initialize();
		virtual void updateBegin(float time);
		virtual void mainUpdate(float time);
		virtual void updateEnd(float time);

		EntityFilter _filter;
	};
}

