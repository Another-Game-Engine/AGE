#pragma once

#include <Systems/System.h>

#define MAX_SCENE_NAME_LENGTH 255

namespace AGE
{
	namespace WE
	{
		class EntityManager : public System
		{
		public:
			EntityManager(AScene *scene);
			virtual ~EntityManager();
		private:
			EntityFilter _filter;
			EntityFilter _meshRenderers;

			std::vector<const char*> _entityNames;
			std::vector<Entity> _entities;
			int _selectedEntity;
			char _sceneName[MAX_SCENE_NAME_LENGTH];

			virtual void updateBegin(float time);
			virtual void updateEnd(float time);
			virtual void mainUpdate(float time);
			virtual bool initialize();
		};
	}
}