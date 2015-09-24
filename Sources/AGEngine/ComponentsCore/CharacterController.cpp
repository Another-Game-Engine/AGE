#include "CharacterController.hh"

#include <Entity/EntityData.hh>
#include <Core/AScene.hh>
#include <Physics/PhysicsInterface.hpp>

namespace AGE
{
	void CharacterController::init(void)
	{
		controller = entity->getScene()->getInstance<Physics::PhysicsInterface>()->getWorld()->createCharacterController();
	}

#if defined(EDITOR_ENABLED)
	void CharacterController::editorCreate(void)
	{

	}

	void CharacterController::editorDelete(void)
	{

	}

	bool CharacterController::editorUpdate(void)
	{
		bool modified = false;

		return (modified);
	}
#endif

	void CharacterController::_copyFrom(const ComponentBase *model)
	{
		(void)(model);
		assert(false && "Paul, je te laisse le remplir");
	}

}
