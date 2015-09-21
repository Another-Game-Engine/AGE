#include "FPSCharacter.hh"

#include <Entity/EntityData.hh>
#include <Core/AScene.hh>
#include <Physics/PhysicsInterface.hpp>

namespace AGE
{
	void FPSCharacter::init(void)
	{
	}

	void FPSCharacter::editorCreate(void)
	{

	}

	void FPSCharacter::editorDelete(void)
	{

	}

	bool FPSCharacter::editorUpdate(void)
	{
		bool modified = false;

		return (modified);
	}

	void FPSCharacter::_copyFrom(const ComponentBase *model)
	{
		(void)(model);
		assert(false && "Paul, je te laisse le remplir");
	}

}
