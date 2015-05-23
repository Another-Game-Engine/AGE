#pragma once

#include <Utils/Dependency.hpp>
#include <Utils/Directory.hpp>
#include <Core/SceneChunkSerialization.hpp>
#include <map>

class Entity;
class AScene;

namespace AGE
{
	class ArchetypeLibrary : public Dependency<ArchetypeLibrary>
	{
	public:

		struct ArchetypeFileModel
		{
			SceneChunkSerialization collection;
			std::string name;

			template <typename Archive>
			void serialize(Archive &ar, const std::uint32_t version)
			{
				ar(CEREAL_NVP(name));
				ar(CEREAL_NVP(collection));
			}
		};

		void setPath(const std::string &library);
		const std::string &getPath() const;
		void load();
		void save();
		void clear();
		void addArchetype(const std::string &name, const Entity &entity);
		std::shared_ptr<AScene> getScene();
		void enableScene();
		void disableScene();
		const std::map<std::string, ArchetypeFileModel> &getLibrary() const;
		// Entity needs to be created before ! (scene->createEntity())
		bool spawn(const std::string &archetypeName, Entity &entity) const;
	private:
		std::string _libraryPath = "";
		std::shared_ptr<AScene> _archetypesScene = nullptr;
		std::map<std::string, ArchetypeFileModel> _library;
	};
}

CEREAL_CLASS_VERSION(AGE::ArchetypeLibrary::ArchetypeFileModel, 0)
