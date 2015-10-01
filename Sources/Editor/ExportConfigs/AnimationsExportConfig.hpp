#pragma once

#include <list>
#include <string>
#include <memory>

#include <Utils/Serialization/SerializationArchives.hpp>
#include <cereal/types/list.hpp>

namespace AGE
{
	struct AnimationsExportConfig
	{
		AnimationsExportConfig();

		struct AnimationNode
		{
			std::string rawAnimationPath;
			std::string animationName; //name.aage
			AnimationNode();

			template <typename Archive>
			void serialize(Archive &ar, std::uint32_t const version)
			{
				ar(CEREAL_NVP(rawAnimationPath));
				ar(CEREAL_NVP(animationName));
			}
		};

		std::string name;

		bool editableName = true;

		std::string rawMeshPath; // used for skin and skeleton
		std::string meshExportName; // name.skage + name.sage

		std::list<AnimationNode> animations;

		//used for imgui
		static const std::size_t maxPathLength = 1024;
		static const std::size_t maxNameLength = 256;

		void displayGui(bool &save, bool &close, bool &cook);

		template <typename Archive>
		void serialize(Archive &ar, std::uint32_t const version)
		{
			ar(CEREAL_NVP(name));
			ar(CEREAL_NVP(rawMeshPath));
			ar(CEREAL_NVP(meshExportName));
			ar(cereal::make_nvp("Animations", animations));
		}
	};

	class AnimationExportConfigManager
	{
		std::shared_ptr<AnimationsExportConfig> _config = nullptr;
		std::string _rootPath;
		std::string _filePath;
	public:
		static AnimationExportConfigManager *getInstance();
		void update();
		void create(const std::string &rootPath);
		void edit(const std::string &filePath);
	};
}

CEREAL_CLASS_VERSION(AGE::AnimationsExportConfig, 0)
CEREAL_CLASS_VERSION(AGE::AnimationsExportConfig::AnimationNode, 0)