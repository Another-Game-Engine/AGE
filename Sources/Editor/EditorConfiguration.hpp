#pragma once

#include <FileUtils/FileSystemHelpers.hpp>
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
				std::string _rawAssetsDirectory = "../../Datas/Assets/Raw/";
				std::string _cookedAssetsDirectory = "../../Datas/Assets/Serialized/";
				std::string _saveSceneFolder = "../../Datas/Saves/";
				std::string _exportSceneFolder = "../../Datas/DemoScenes/";
				std::string _saveArchetypeFolder = "../../Datas/Archetypes/";
				std::string _exportArchetypeFolder = "../../Datas/Archetypes/";

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
			static std::shared_ptr<Configurations> _getConfigurations();
			static std::shared_ptr<Configurations> _configurations;
		public:
			static const std::string &GetRawDirectory();
			static const std::string &GetCookedDirectory();
			static const std::string &GetEditedSceneDirectory();
			static const std::string &GetExportedSceneDirectory();
			static const std::string &GetEditedArchetypeDirectory();
			static const std::string &GetExportedArchetypeDirectory();
			static std::vector<const char*> getScenesName();
			static const std::vector<std::string> &getScenesPath();
			static const std::string getSelectedScenePath();
			static const std::string getSelectedSceneName();
			static int &getSelectedSceneIndex();
			static void RefreshScenesDirectoryListing();
		private:
			static void _saveConfigurations();
		};
	}
}

CEREAL_CLASS_VERSION(AGE::WE::EditorConfiguration::Configurations, 0)