#include "BulletPhysics.hpp"

#ifdef interface
# undef interface
#endif

extern "C"
{
	__declspec(dllexport) AGE::Physics::PhysicsInterface *CreateInterface(void)
	{
		return new AGE::Physics::BulletPhysics;
	}

	__declspec(dllexport) void DestroyInterface(AGE::Physics::PhysicsInterface *interface)
	{
		delete interface;
	}
}
