#pragma once

#include <unordered_map>

#include <Physics/EngineTypeToPluginName.hpp>

namespace AGE
{
	namespace Physics
	{
		using HashTable = std::unordered_map < EngineTypes, std::string >;

		static const HashTable PhysicsEngineTypeToPluginNameTable
		{
			{ EngineTypes::Bullet, "Physics_Bullet" },
			{ EngineTypes::PhysX, "Physics_PhysX" },
			{ EngineTypes::Null, "Physics_Null" }
		};

		std::string GetPluginNameForEngine(EngineTypes physicsEngineType)
		{
			HashTable::const_iterator found = PhysicsEngineTypeToPluginNameTable.find(physicsEngineType);
			return found != PhysicsEngineTypeToPluginNameTable.end() ? found->second : PhysicsEngineTypeToPluginNameTable.find(EngineTypes::Null)->second;
		}
	}
}