#include "Precompiled.hpp"

#include <iostream>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReasonForCall, LPVOID lpReserved)
{
	switch (ulReasonForCall)
	{
		case DLL_PROCESS_ATTACH:
			std::cout << "Loaded" << std::endl;
			break;
		case DLL_PROCESS_DETACH:
			std::cout << "Unloaded" << std::endl;
			break;
		default:
			break;
	}
	return TRUE;
}