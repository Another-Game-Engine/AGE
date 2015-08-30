#pragma once

#include <Core/AScene.hh>
#include <Core/Engine.hh>
#include <memory>

#include <Systems/PhysicsSystem.hpp>
#include "../Plugins/Physics/PhysX/PhysXPhysics.hpp"

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
			addSystem<PhysicsSystem>(3, Physics::EngineType::PhysX, getInstance<AGE::AssetsManager>());
			getInstance<Physics::PhysicsInterface>()->getWorld()->setGravity(0, 0, 0);
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