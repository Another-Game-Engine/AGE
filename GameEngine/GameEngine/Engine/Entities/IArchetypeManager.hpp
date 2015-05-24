#pragma once

#include <string>

namespace AGE
{
	class Entity;

	class IArchetypeManager
	{
	public:
		virtual void save() = 0;
		virtual void load() = 0;
		virtual void loadOne(const std::string &name) = 0;
		virtual void addOne(const std::string &name, Entity &entity) = 0;
		virtual void spawn(Entity &entity, const std::string &name) = 0;
		inline void setLibraryFolder(const std::string &path) { _libraryFolder = path; }
	protected:
		std::string _libraryFolder = "";
	};
}