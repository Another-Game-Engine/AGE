#pragma once

#include <Systems/System.h>

namespace AGE
{
	namespace WE
	{
		class EntityManager : public System
		{
		public:
			EntityManager(std::weak_ptr<AScene> &&scene);
			virtual ~EntityManager();
		private:
			EntityFilter _filter;
			std::vector<const char*> _entityNames;
			std::vector<Entity> _entities;
			int _selectedEntity;

			virtual void updateBegin(double time);
			virtual void updateEnd(double time);
			virtual void mainUpdate(double time);
			virtual bool initialize();
		};
	}
}