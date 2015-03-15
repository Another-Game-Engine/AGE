#pragma once

#include <Systems/System.h>

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
			std::vector<const char*> _entityNames;
			std::vector<Entity> _entities;
			int _selectedEntity;

			virtual void updateBegin(float time);
			virtual void updateEnd(float time);
			virtual void mainUpdate(float time);
			virtual bool initialize();
		};
	}
}