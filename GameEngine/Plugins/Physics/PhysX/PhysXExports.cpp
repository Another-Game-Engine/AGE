#include "PhysXPhysics.hpp"

extern "C"
{
	__declspec(dllexport) AGE::Physics::PhysicsInterface *CreateInterface(void)
	{
		return static_cast<AGE::Physics::PhysicsInterface *>(new AGE::Physics::PhysXPhysics);
	}

	__declspec(dllexport) void DestroyInterface(AGE::Physics::PhysicsInterface *interface)
	{
		delete interface;
	}
}
