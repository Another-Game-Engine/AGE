#pragma once

#include <vector>
#include <string>
#include <map>

#include <Entity/EntityTypedef.hpp>
#include <Entity/Entity.hh>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

namespace AGE
{
	struct ComponentBase;
	class Entity;
	struct BinaryEntity;

	struct ReadableEntity
	{
		std::vector <std::size_t> children;
		std::vector <ComponentBase*> components;
		std::vector <ComponentType> componentTypes;
		std::vector<std::string> archetypesDependency;
		Entity entity;
		std::map<ComponentType, std::size_t> *typesMap = nullptr; // used to unserialize

		ReadableEntity();
		~ReadableEntity();
		void save(cereal::JSONOutputArchive  &ar, const std::uint32_t version) const;
		void load(cereal::JSONInputArchive &ar, const std::uint32_t version);
		BinaryEntity toBinary();
	};
}

CEREAL_CLASS_VERSION(AGE::ReadableEntity, 0);