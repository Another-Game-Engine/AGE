#pragma once

#include <Entity/IArchetypeManager.hpp>
#include <map>

namespace AGE
{
	class AScene;
	class Entity;

	class ArchetypeManager : public IArchetypeManager
	{
	public:
		ArchetypeManager();
		virtual ~ArchetypeManager();
		virtual void load();
		virtual void loadOne(const std::string &name);
		virtual void spawn(Entity &entity, const std::string &name);

		AScene *getScene();
		void enableScene();
		void disableScene();
	private:
		std::map<std::string, Entity> _archetypesCollection;
		AScene *_archetypesScene;
	};
}