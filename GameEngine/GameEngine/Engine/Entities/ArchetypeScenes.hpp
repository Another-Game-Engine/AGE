#pragma once

#include <Core/AScene.hh>
#include <Core/Engine.hh>
#include <memory>

///////////////////////////
// This scene is used to keep archetypes in
// a different scene that others
//
//////////////////////////

namespace AGE
{
	class ArchetypeScene : public AScene
	{
	public:
		static const std::string Name;

		ArchetypeScene(AGE::Engine *engine)
			:AScene(engine)
		{}

		virtual ~ArchetypeScene(void)
		{}

		virtual bool _userStart()
		{
			return true;
		}

		virtual bool _userUpdateBegin(float time)
		{

			return true;
		}

		virtual bool _userUpdateEnd(float time)
		{
			return true;
		}
	};
}