#pragma once

#include <unordered_map>

#include <Physics/EngineTypeToPluginName.hpp>

namespace AGE
{
	namespace Physics
	{
		std::string GetPluginNameForEngine(EngineTypes physicsEngineType)
		{
			using HashTable = std::unordered_map < EngineTypes, std::string > ;
			static const HashTable physicsEngineTypeToPluginNameTable
			{
				{ EngineTypes::Bullet, "Physics_Bullet" },
				{ EngineTypes::PhysX, "Physics_PhysX" },
				{ EngineTypes::Null, "Physics_Null" }
			};
			HashTable::const_iterator found = physicsEngineTypeToPluginNameTable.find(physicsEngineType);
			return found != physicsEngineTypeToPluginNameTable.end() ? found->second : physicsEngineTypeToPluginNameTable.find(EngineTypes::Null)->second;
		}
	}
}