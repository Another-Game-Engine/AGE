#pragma once

#include <Utils/Dependency.hpp>
#include <Utils/Directory.hpp>
#include <Utils/FileSystemHelpers.hpp>
#include <Entities/Entity.hh>
#include <cereal/types/set.hpp>
#include <Utils/Path.hpp>

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
				ar(name);
				ar(collection);
			}
		};

		void setPath(const std::string &library)
		{
			_libraryPath = library;
		}

		const std::string &getPath() const
		{
			return _libraryPath;
		}

		void load()
		{
			Directory dir;
			const bool success = dir.open(_libraryPath.c_str());
			AGE_ASSERT(success);
			for (auto it = dir.recursive_begin(); it != dir.recursive_end(); ++it)
			{
				if (Directory::IsFile(*it))
				{
					auto extension = AGE::FileSystemHelpers::GetExtension(*it);

					if (extension != "archetype")
					{
						continue;
					}

					ArchetypeFileModel model;
					model.collection = SceneChunkSerialization::CreateForLoad(getScene().get());
					{
						std::ifstream file(*it, std::ios::binary);
						auto ar = cereal::JSONInputArchive(file);
						ar(model);
						file.close();
					}

					_library[model.name] = model;
				}
			}
			dir.close();
		}

		void save()
		{
			for (auto &e : _library)
			{
				std::ofstream file(std::string(_libraryPath + "/" + e.second.name + ".archetype").c_str(), std::ios::binary);
				{
					auto ar = cereal::JSONOutputArchive(file);
					ar(e.second);
				}
				file.close();
			}
		}

		void clear()
		{
			_library.clear();
		}

		void addArchetype(const std::string &name, const std::set<Entity> &entities)
		{
			_library[name] = ArchetypeFileModel();
			auto &e = _library[name];

			e.name = name;
			e.collection = AScene::BuildSceneChunkFromSelection(entities);
		}

		std::shared_ptr<AScene> getScene()
		{
			if (_archetypesScene == nullptr)
			{
				_archetypesScene = std::make_shared<ArchetypeScene>(GetEngine());
				GetEngine()->addScene(_archetypesScene, "ARCHETYPES_SCENE");
				GetEngine()->initScene("ARCHETYPES_SCENE");
				GetEngine()->enableScene("ARCHETYPES_SCENE", 0);
			}
			return _archetypesScene;
		}

		void enableScene()
		{
			GetEngine()->enableScene("ARCHETYPES_SCENE", 0);
		}

		void disableScene()
		{
			GetEngine()->enableScene("ARCHETYPES_SCENE", 0);
		}

		const std::map<std::string, ArchetypeFileModel> &getLibrary() const
		{
			return _library;
		}
	private:
		std::string _libraryPath = "";
		std::shared_ptr<AScene> _archetypesScene = nullptr;
		std::map<std::string, ArchetypeFileModel> _library;
	};
}

CEREAL_CLASS_VERSION(AGE::ArchetypeLibrary::ArchetypeFileModel, 0)
