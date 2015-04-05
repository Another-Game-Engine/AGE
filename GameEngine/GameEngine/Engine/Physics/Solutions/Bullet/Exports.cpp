#include "Precompiled.hpp"

extern "C"
{
	EXPORT_API AGE::Physics::PluginInterface *CreateInterface(void)
	{
		return new AGE::Physics::Plugin;
	}

	EXPORT_API void DestroyInterface(AGE::Physics::PluginInterface *interface)
	{
		delete interface;
	}
}
