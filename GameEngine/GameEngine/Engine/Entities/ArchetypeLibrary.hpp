#pragma once

#include <Utils/Dependency.hpp>

namespace AGE
{
	class ArchetypeLibrary : public Dependency<ArchetypeLibrary>
	{
	public:

		struct ArchetypeFileModel
		{
			std::set<Entity> collection;
			std::string name;

			template <typename Archive>
			void serialize(Archive &ar, const std::uint32_t version)
			{
				ar(name);
				ar(collection);
			}
		};

		bool setPath(const std::string &library)
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

					{
						std::ifstream file(Path::BaseName(*it), std::ios::binary);
						auto ar = cereal::JSONInputArchive(file);
						ar(model);
						file.close();
					}

					_library[model.name] = model;
				}
			}
			dir.close();
		}

		bool save()
		{
			Directory dir;
			bool success = dir.open(_libraryPath.c_str());
			AGE_ASSERT(success);

			for (auto &e : _library)
			{
				success = dir.open(e.name + ".archetype");
				AGE_ASSERT(success);


			}
		}

		void clear()
		{}

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
	private:
		std::string _libraryPath = "";
		std::shared_ptr<AScene> _archetypesScene = nullptr;
		std::map<std::string, ArchetypeFileModel> _library;
	};
}

CEREAL_CLASS_VERSION(AGE::ArchetypeLibrary::ArchetypeFileModel, 0)
