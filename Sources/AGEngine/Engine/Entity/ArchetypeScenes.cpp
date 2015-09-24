#include "ArchetypeScenes.hpp"
#include "SystemsCore/PhysicsSystem.hpp"

namespace AGE
{
	ArchetypeScene::ArchetypeScene(AGE::Engine *engine)
		:AScene(engine)
	{}

	ArchetypeScene::~ArchetypeScene(void)
	{}

	bool ArchetypeScene::_userStart()
	{
		addSystem<PhysicsSystem>(3, Physics::EngineType::PhysX, getInstance<AGE::AssetsManager>(), false);
		getInstance<Physics::PhysicsInterface>()->getWorld()->setGravity(0, 0, 0);
		return true;
	}

	bool ArchetypeScene::_userUpdateBegin(float time)
	{
		return true;
	}

	bool ArchetypeScene::_userUpdateEnd(float time)
	{
		return true;
	}
}