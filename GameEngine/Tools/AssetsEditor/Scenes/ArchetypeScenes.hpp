#pragma once

#include <Core/AScene.hh>
#include <Core/Engine.hh>
#include <memory>
#include <systems/AssetsAndComponentRelationsSystem.hpp>

///////////////////////////
// This scene is used to keep archetypes in
// a different scene that World Editor
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
			, _filter(this)
		{}

		virtual ~ArchetypeScene(void)
		{}

		virtual bool _userStart()
		{
			addSystem<WE::AssetsAndComponentRelationsSystem>(0);
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

	private:
		EntityFilter _filter;
	};
}