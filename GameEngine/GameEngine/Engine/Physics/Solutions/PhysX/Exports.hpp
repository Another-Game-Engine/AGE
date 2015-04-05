#pragma once

#ifdef _EXPORTS
# define EXPORT_API __declspec(dllexport)
#else
# define EXPORT_API __declspec(dllimport)
#endif

#include "Plugin.hpp"

extern "C"
{
	EXPORT_API AGE::Physics::PluginInterface *CreateInterface(void);

	EXPORT_API void DestroyInterface(AGE::Physics::PluginInterface *interface);
}
