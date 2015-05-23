#pragma once

#include <Utils/FileSystemHelpers.hpp>
#include <Utils/Serialization/SerializationArchives.hpp>
#include <fstream>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <Utils/Directory.hpp>
#include <Utils/Debug.hpp>
#include <Utils/Path.hpp>
#include <vector>
#include <list>

namespace AGE
{
	namespace WE
	{
		class EditorConfiguration
		{
		public:
			friend class cereal::access;
			struct Configurations
			{
				std::string _rawAssetsDirectory = "../../Assets/Raw/";
				std::string _cookedAssetsDirectory = "../../Assets/Serialized/";
				std::string _saveSceneFolder = "../../Tools/AssetsEditor/Saves/";
				std::string _exportSceneFolder = "../../EngineCoreTest/DemoScenes/";
				std::string _saveArchetypeFolder = "../../Tools/AssetsEditor/Archetypes/";
				std::string _exportArchetypeFolder = "../../EngineCoreTest/Archetypes/";

				int _selectedScene = 0;

				// not serialized
				std::vector<std::string> _scenesPaths;
				std::list<std::string> _scenesNamesCache;
				std::vector<const char*> _scenesNames;

				friend class cereal::access;

				template <typename Archive>
				void serialize(Archive &ar, std::uint32_t const version)
				{
					ar(cereal::make_nvp("Raw assets directory", _rawAssetsDirectory));
					ar(cereal::make_nvp("Cooked assets directory", _cookedAssetsDirectory));
					ar(cereal::make_nvp("Edited scenes directory", _saveSceneFolder));
					ar(cereal::make_nvp("Exported scenes directory", _exportSceneFolder));
					ar(cereal::make_nvp("Saved archetypes directory", _saveArchetypeFolder));
					ar(cereal::make_nvp("Exported archetypes directory", _exportArchetypeFolder));
				}
			};

		private:

			static std::shared_ptr<Configurations> _getConfigurations()
			{
				if (_configurations == nullptr)
				{
					if (!AGE::FileSystemHelpers::AgeExists("../../EditorConfiguration.json"))
					{
						_configurations = std::make_shared<Configurations>();
						_saveConfigurations();
					}
					else
					{
						std::ifstream file("../../EditorConfiguration.json", std::ios::binary);
						assert(file.is_open());
						{
							auto ar = cereal::JSONInputArchive(file);
							ar(_configurations);
						}
						file.close();
					}
				}
				return _configurations;
			}

			static std::shared_ptr<Configurations> _configurations;
		public:
			static const std::string &GetRawDirectory()
			{
				return _getConfigurations()->_rawAssetsDirectory;
			}
			static const std::string &GetCookedDirectory()
			{
				return _getConfigurations()->_cookedAssetsDirectory;
			}
			static const std::string &GetEditedSceneDirectory()
			{
				return _getConfigurations()->_saveSceneFolder;
			}
			static const std::string &GetExportedSceneDirectory()
			{
				return _getConfigurations()->_exportSceneFolder;
			}
			static const std::string &GetEditedArchetypeDirectory()
			{
				return _getConfigurations()->_saveArchetypeFolder;
			}
			static const std::string &GetExportedArchetypeDirectory()
			{
				return _getConfigurations()->_exportArchetypeFolder;
			}
			static std::vector<const char*> getScenesName()
			{
				return _getConfigurations()->_scenesNames;
			}
			static const std::vector<std::string> &getScenesPath()
			{
				return _getConfigurations()->_scenesPaths;
			}
			static const std::string getSelectedScenePath()
			{
				if (_getConfigurations()->_selectedScene >= _getConfigurations()->_scenesPaths.size())
				{
					return "";
				}
				return _getConfigurations()->_scenesPaths[_getConfigurations()->_selectedScene];
			}
			static const std::string getSelectedSceneName()
			{
				if (_getConfigurations()->_selectedScene >= _getConfigurations()->_scenesNames.size())
				{
					return "";
				}
				return _getConfigurations()->_scenesNames[_getConfigurations()->_selectedScene];
			}
			static int &getSelectedSceneIndex()
			{
				return _getConfigurations()->_selectedScene;
			}
			static void RefreshScenesDirectoryListing()
			{
				Directory dir;
				auto editedSceneDirectory = GetEditedSceneDirectory();
				const bool succeed = dir.open(editedSceneDirectory.c_str());
				AGE_ASSERT(succeed && "Impossible to open directory");

				_getConfigurations()->_scenesPaths.clear();
				_getConfigurations()->_scenesNames.clear();
				_getConfigurations()->_scenesNamesCache.clear();

				for (auto it = dir.recursive_begin(); it != dir.recursive_end(); ++it)
				{
					if (Directory::IsFile(*it))
					{
						auto file = std::string(*it);
						auto find = file.find("_scene_description.json");
						if (find != std::string::npos)
						{
							_getConfigurations()->_scenesNamesCache.push_back(Path::BaseName(file.substr(0, find).c_str()));
							_getConfigurations()->_scenesNames.push_back(_getConfigurations()->_scenesNamesCache.back().c_str());
							_getConfigurations()->_scenesPaths.push_back(_getConfigurations()->_saveSceneFolder + _getConfigurations()->_scenesNames.back());
						}
					}
				}
				dir.close();

				auto &selectedIndex = _getConfigurations()->_selectedScene;
				if (selectedIndex >= _getConfigurations()->_scenesNames.size())
				{
					selectedIndex = 0;
					return;
				}
			}
			private:
			static void _saveConfigurations()
			{
				RefreshScenesDirectoryListing();

				std::ofstream file("../../EditorConfiguration.json", std::ios::binary);
				assert(file.is_open());
				{
					auto ar = cereal::JSONOutputArchive(file);
					ar(_configurations);
				}
				file.close();
			}
		};
	}
}

CEREAL_CLASS_VERSION(AGE::WE::EditorConfiguration::Configurations, 0)