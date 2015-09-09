#pragma once

#include <string>
#include <Utils/Dependency.hpp>

namespace AGE
{
	class Entity;

	class IArchetypeManager : public Dependency < IArchetypeManager >
	{
	public:
		virtual void load() = 0;
		virtual void loadOne(const std::string &name) = 0;
		virtual void spawn(Entity &entity, const std::string &name) = 0;
		virtual void updateArchetype(const std::string &) {}
		inline void setLibraryFolder(const std::string &path) { _libraryFolder = path; }
	protected:
		std::string _libraryFolder = "";
	};
}