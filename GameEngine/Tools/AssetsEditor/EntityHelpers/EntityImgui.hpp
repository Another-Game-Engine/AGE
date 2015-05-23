#pragma once

#include <vector>

namespace AGE
{
	class Entity;
	class AScene;

	namespace WE
	{
		void listEntityTree(Entity &entity, std::vector<const char*> &names, std::vector<Entity*> &entities);

		//return true if entity has been modified
		bool displayEntity(Entity &entity, AScene *scene);

		//return true if entity has been modified
		bool recursiveDisplayList(Entity &entity, Entity *& selectedEntity, bool &selectParent, bool editableHierarchy = true);
	}
}