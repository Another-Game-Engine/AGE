#pragma once

#include <unordered_map>

#include <Physics/EngineTypeToPluginName.hpp>

namespace AGE
{
	namespace Physics
	{
		using HashTable = std::unordered_map < EngineType, std::string >;

		static const HashTable PhysicsEngineTypeToPluginNameTable
		{
			{ EngineType::Bullet, "AGEPlugin_Bullet" },
			{ EngineType::PhysX, "AGEPlugin_PhysX" },
			{ EngineType::Null, "AGEPlugin_Null" }
		};

		std::string GetPluginNameForEngine(EngineType physicsEngineType)
		{
			HashTable::const_iterator found = PhysicsEngineTypeToPluginNameTable.find(physicsEngineType);
			return found != PhysicsEngineTypeToPluginNameTable.end() ? found->second : PhysicsEngineTypeToPluginNameTable.find(EngineType::Null)->second;
		}
	}
}