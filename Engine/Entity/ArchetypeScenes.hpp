#pragma once

#include <Core/AScene.hh>

///////////////////////////
// This scene is used to keep archetypes in
// a different scene that others
//
//////////////////////////

namespace AGE
{
	class Engine;

	class ArchetypeScene : public AScene
	{
	public:
		static const std::string Name;

		ArchetypeScene(AGE::Engine *engine);

		virtual ~ArchetypeScene(void);

		virtual bool _userStart();

		virtual bool _userUpdateBegin(float time);

		virtual bool _userUpdateEnd(float time);
	};
}