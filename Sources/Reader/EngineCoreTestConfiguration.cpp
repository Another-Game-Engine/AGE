#include "EngineCoreTestConfiguration.hpp"

std::shared_ptr<AGE::ECTConfigurations> AGE::EngineCoreTestConfiguration::_configurations = nullptr;
bool AGE::EngineCoreTestConfiguration::_dirty = true;

namespace AGE
{
	EngineCoreTestConfiguration::~EngineCoreTestConfiguration()
	{
		saveConfigurations();
	}

	std::shared_ptr<ECTConfigurations> EngineCoreTestConfiguration::_getConfigurations()
	{
		if (_configurations == nullptr)
		{
			if (!FileUtils::AgeExists("EngineCoreTestConfiguration.json"))
			{
				_configurations = std::make_shared<ECTConfigurations>();
				saveConfigurations();
			}
			else
			{
				std::ifstream file("EngineCoreTestConfiguration.json");
				assert(file.is_open());

				// if file is empty
				if (file.peek() == std::ifstream::traits_type::eof())
				{
					_configurations = std::make_shared<ECTConfigurations>();
					saveConfigurations();
				}
				else
				{
					auto ar = cereal::JSONInputArchive(file);
					ar(_configurations);
				}
				file.close();
			}
		}
		if (_dirty)
		{
			saveConfigurations();
		}
		return _configurations;
	}

	const std::string &EngineCoreTestConfiguration::GetCookedDirectory()
	{
		return _getConfigurations()->_cookedAssetsDirectory;
	}
	const std::string &EngineCoreTestConfiguration::GetExportedSceneDirectory()
	{
		return _getConfigurations()->_exportSceneFolder;
	}
	const std::string &EngineCoreTestConfiguration::GetExportedArchetypeDirectory()
	{
		return _getConfigurations()->_exportArchetypeFolder;
	}
	std::vector<const char*> EngineCoreTestConfiguration::getScenesName()
	{
		return _getConfigurations()->_scenesNames;
	}
	const std::vector<std::string> &EngineCoreTestConfiguration::getScenesPath()
	{
		return _getConfigurations()->_scenesPaths;
	}
	const std::string EngineCoreTestConfiguration::getSelectedScenePath()
	{
		if (_getConfigurations()->_selectedScene >= _getConfigurations()->_scenesPaths.size())
		{
			return "";
		}
		return _getConfigurations()->_scenesPaths[_getConfigurations()->_selectedScene];
	}
	const std::string EngineCoreTestConfiguration::getSelectedSceneName()
	{
		if (_getConfigurations()->_selectedScene >= _getConfigurations()->_scenesNames.size())
		{
			return "";
		}
		return _getConfigurations()->_scenesNames[_getConfigurations()->_selectedScene];
	}
	int &EngineCoreTestConfiguration::getSelectedSceneIndex()
	{
		auto &res = _getConfigurations()->_selectedScene;
		return res;
	}

	void EngineCoreTestConfiguration::RefreshScenesDirectoryListing()
	{
		_dirty = true;

		Directory dir;
		auto aeffacer = GetExportedSceneDirectory();
		const bool succeed = dir.open(GetExportedSceneDirectory().c_str());
		AGE_ASSERT(succeed && "Impossible to open directory");

		_getConfigurations()->_scenesPaths.clear();
		_getConfigurations()->_scenesNames.clear();
		_getConfigurations()->_scenesNamesCache.clear();

		for (auto it = dir.recursive_begin(); it != dir.recursive_end(); ++it)
		{
			if (Directory::IsFile(*it))
			{
				auto file = std::string(*it);
				auto find = file.find(".scene");
				if (find != std::string::npos)
				{
					_getConfigurations()->_scenesNamesCache.push_back(Path::BaseName(file.substr(0, find).c_str()));
					_getConfigurations()->_scenesNames.push_back(_getConfigurations()->_scenesNamesCache.back().c_str());
					_getConfigurations()->_scenesPaths.push_back(_getConfigurations()->_exportSceneFolder + _getConfigurations()->_scenesNames.back() + ".scene");
				}
			}
		}
		dir.close();

		auto &selectedIndex = _getConfigurations()->_selectedScene;
		selectedIndex = 0;

		if (!_getConfigurations()->_lastSceneOpened.empty())
		{
			for (auto &e : _getConfigurations()->_scenesNames)
			{
				if (e == _getConfigurations()->_lastSceneOpened)
				{
					break;
				}
				++selectedIndex;
			}
		}

		if (selectedIndex >= _getConfigurations()->_scenesNames.size())
		{
			selectedIndex = 0;
		}

		_dirty = true;
	}

	void EngineCoreTestConfiguration::saveConfigurations()
	{
		if (_configurations && !_configurations->_scenesNames.empty())
		{
			_configurations->_lastSceneOpened = _configurations->_scenesNames[_configurations->_selectedScene];
		}

		std::ofstream file("EngineCoreTestConfiguration.json");
		assert(file.is_open());
		{
			auto ar = cereal::JSONOutputArchive(file);
			ar(_configurations);
		}
		file.close();
		_dirty = false;
	}
}