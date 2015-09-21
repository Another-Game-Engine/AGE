#pragma once

#include <FileUtils/FileUtils/FileSystemHelpers.hpp>
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
	struct ECTConfigurations
	{
		std::string _cookedAssetsDirectory = "../../GameEngine/Assets/Serialized/";
		std::string _exportSceneFolder = "../../Reader/DemoScenes/";
		std::string _exportArchetypeFolder = "../../GameEngine/Archetypes/";
		std::string _lastSceneOpened = "";
		int _selectedScene = 0;

		// not serialized
		std::vector<std::string> _scenesPaths;
		std::list<std::string> _scenesNamesCache;
		std::vector<const char*> _scenesNames;

		friend class cereal::access;

		template <typename Archive>
		void serialize(Archive &ar, std::uint32_t const version)
		{
			ar(cereal::make_nvp("Cooked assets directory", _cookedAssetsDirectory));
			ar(cereal::make_nvp("Exported scenes directory", _exportSceneFolder));
			if (version == 0)
			{
				ar(cereal::make_nvp("deprecated", _selectedScene));
			}
			else
			{
				ar(cereal::make_nvp("Last scene opened", _lastSceneOpened));
			}
			ar(cereal::make_nvp("Archetypes directory", _exportArchetypeFolder));
		}
	};

	class EngineCoreTestConfiguration
	{
	public:
		~EngineCoreTestConfiguration();

		static bool _dirty;

		static std::shared_ptr<ECTConfigurations> _getConfigurations();
		static std::shared_ptr<ECTConfigurations> _configurations;
	public:
		static const std::string &GetCookedDirectory();
		static const std::string &GetExportedSceneDirectory();
		static const std::string &GetExportedArchetypeDirectory();
		static std::vector<const char*> getScenesName();
		static const std::vector<std::string> &getScenesPath();
		static const std::string getSelectedScenePath();
		static const std::string getSelectedSceneName();
		static int &getSelectedSceneIndex();
		static void RefreshScenesDirectoryListing();
		static void saveConfigurations();
	};
}

CEREAL_CLASS_VERSION(AGE::ECTConfigurations, 1);