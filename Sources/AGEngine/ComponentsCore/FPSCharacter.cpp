#include "FPSCharacter.hh"

#include <Entity/EntityData.hh>
#include <Core/AScene.hh>
#include <Physics/PhysicsInterface.hpp>

namespace AGE
{
	void FPSCharacter::init(void)
	{
	}

#if defined(EDITOR_ENABLED)
	bool FPSCharacter::editorUpdate(void)
	{
		bool modified = false;

		return (modified);
	}
#endif
	void FPSCharacter::_copyFrom(const ComponentBase *model)
	{
		(void)(model);
		assert(false && "Paul, je te laisse le remplir");
	}

}
