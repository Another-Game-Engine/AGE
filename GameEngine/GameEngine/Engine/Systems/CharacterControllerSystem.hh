#pragma once

#include <System/System.h>
#include <Core/EntityFilter.hpp>

namespace AGE
{
	class CharacterControllerSystem : public System
	{
	public:
		CharacterControllerSystem() = delete;
		CharacterControllerSystem(AScene *scene, bool isEditor);
		~CharacterControllerSystem() = default;

	private:
		virtual bool initialize();
		virtual void updateBegin(float time);
		virtual void mainUpdate(float time);
		virtual void updateEnd(float time);

		EntityFilter _filter;
		bool _isEditor;
	};
}

