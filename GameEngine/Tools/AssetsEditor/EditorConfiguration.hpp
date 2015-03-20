#pragma once

#include <Utils/FileSystemHelpers.hpp>
#include <Utils/Serialization/SerializationArchives.hpp>
#include <fstream>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>

namespace AGE
{
	namespace WE
	{
		class EditorConfiguration
		{
		private:
			friend class cereal::access;
			struct Configurations
			{
				std::string _rawAssetsDirectory = "../../Assets/Raw/";
				std::string _cookedAssetsDirectory = "../../Assets/Serialized/";
				std::string _saveSceneFolder = "../../Tools/AssetsEditor/Saves/";
				std::string _exportSceneFolder = "../../EngineCoreTest/DemoScenes/";
				friend class cereal::access;

				template <typename Archive>
				void serialize(Archive &ar, std::uint32_t const version)
				{
					ar(cereal::make_nvp("Raw assets directory", _rawAssetsDirectory));
					ar(cereal::make_nvp("Cooked assets directory", _cookedAssetsDirectory));
					ar(cereal::make_nvp("Edited scenes directory", _saveSceneFolder));
					ar(cereal::make_nvp("Exported scenes directory", _exportSceneFolder));
				}
			};

			static std::shared_ptr<Configurations> _getConfigurations()
			{
				if (_configurations == nullptr)
				{
					if (!AGE::FileSystemHelpers::AgeExists("EditorConfiguration.json"))
					{
						_configurations = std::make_shared<Configurations>();
						_saveConfigurations();
					}
					else
					{
						std::ifstream file("EditorConfiguration.json", std::ios::binary);
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

			static void _saveConfigurations()
			{
				std::ofstream file("EditorConfiguration.json", std::ios::binary);
				assert(file.is_open());
				{
					auto ar = cereal::JSONOutputArchive(file);
					ar(_configurations);
				}
				file.close();
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
		};
	}
}