#include "PhysXPlugin.hpp"

extern "C"
{
	__declspec(dllexport) AGE::Physics::PluginInterface *CreateInterface(void)
	{
		return new AGE::Physics::PhysXPlugin;
	}

	__declspec(dllexport) void DestroyInterface(AGE::Physics::PluginInterface *interface)
	{
		delete interface;
	}
}
