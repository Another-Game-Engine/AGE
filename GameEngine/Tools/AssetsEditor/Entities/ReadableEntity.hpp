#pragma once

#include <vector>
#include <string>
#include <map>

#include <Entities/EntityTypedef.hpp>
#include <Entities/Entity.hh>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>



namespace AGE
{
	struct ComponentBase;
	class Entity;

	struct ReadableEntity
	{
		std::vector <std::size_t> children;
		std::vector <ComponentBase*> components;
		std::vector <ComponentType> componentTypes;
		std::vector<std::string> archetypesDependency;
		Entity entity;
		std::map<ComponentType, std::size_t> *typesMap; // used to unserialize

		ReadableEntity();
		~ReadableEntity();
		void save(cereal::JSONOutputArchive  &ar, const std::uint32_t version) const;
		void load(const cereal::JSONInputArchive &ar, const std::uint32_t version);
	};
}

CEREAL_CLASS_VERSION(AGE::ReadableEntity, 0);