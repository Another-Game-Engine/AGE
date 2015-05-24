#include "EditorConfiguration.hpp"

namespace AGE
{
	namespace WE
	{
		std::shared_ptr<EditorConfiguration::Configurations> EditorConfiguration::_configurations = nullptr;

		std::shared_ptr<EditorConfiguration::Configurations> EditorConfiguration::_getConfigurations()
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

		const std::string &EditorConfiguration::GetRawDirectory()
		{
			return _getConfigurations()->_rawAssetsDirectory;
		}
		const std::string &EditorConfiguration::GetCookedDirectory()
		{
			return _getConfigurations()->_cookedAssetsDirectory;
		}
		const std::string &EditorConfiguration::GetEditedSceneDirectory()
		{
			return _getConfigurations()->_saveSceneFolder;
		}
		const std::string &EditorConfiguration::GetExportedSceneDirectory()
		{
			return _getConfigurations()->_exportSceneFolder;
		}
		const std::string &EditorConfiguration::GetEditedArchetypeDirectory()
		{
			return _getConfigurations()->_saveArchetypeFolder;
		}
		const std::string &EditorConfiguration::GetExportedArchetypeDirectory()
		{
			return _getConfigurations()->_exportArchetypeFolder;
		}
		std::vector<const char*> EditorConfiguration::getScenesName()
		{
			return _getConfigurations()->_scenesNames;
		}
		const std::vector<std::string> &EditorConfiguration::getScenesPath()
		{
			return _getConfigurations()->_scenesPaths;
		}
		const std::string EditorConfiguration::getSelectedScenePath()
		{
			if (_getConfigurations()->_selectedScene >= _getConfigurations()->_scenesPaths.size())
			{
				return "";
			}
			return _getConfigurations()->_scenesPaths[_getConfigurations()->_selectedScene];
		}
		const std::string EditorConfiguration::getSelectedSceneName()
		{
			if (_getConfigurations()->_selectedScene >= _getConfigurations()->_scenesNames.size())
			{
				return "";
			}
			return _getConfigurations()->_scenesNames[_getConfigurations()->_selectedScene];
		}
		int &EditorConfiguration::getSelectedSceneIndex()
		{
			return _getConfigurations()->_selectedScene;
		}
		void EditorConfiguration::RefreshScenesDirectoryListing()
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
		void EditorConfiguration::_saveConfigurations()
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
	}
}
