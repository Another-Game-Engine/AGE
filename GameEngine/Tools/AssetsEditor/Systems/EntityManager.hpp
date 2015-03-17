#pragma once

#include <Systems/System.h>
#include <glm/gtc/quaternion.hpp>
#include <utility>

namespace AGE
{
	namespace WE
	{
		struct Archetype
		{
			std::string name;
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;
			std::vector<size_t> components;
		};

		class EntityManager : public System
		{
		public:
			EntityManager(AScene *scene);
			virtual ~EntityManager();
		private:
			EntityFilter _filter;
			std::vector<const char*> _entityNames;
			std::vector<Entity> _entities;
			std::vector<char const *> _archetypeNames;
			std::vector<Archetype> _archetypes;
			int _selectedEntity;
			int _selectedArchetype;

			virtual void updateBegin(float time);
			virtual void updateEnd(float time);
			virtual void mainUpdate(float time);
			virtual bool initialize();
		};
	}
}