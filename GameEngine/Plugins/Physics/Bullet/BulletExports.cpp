#include "BulletPlugin.hpp"

extern "C"
{
	__declspec(dllexport) AGE::Physics::PluginInterface *CreateInterface(void)
	{
		return new AGE::Physics::BulletPlugin;
	}

	__declspec(dllexport) void DestroyInterface(AGE::Physics::PluginInterface *interface)
	{
		delete interface;
	}
}
