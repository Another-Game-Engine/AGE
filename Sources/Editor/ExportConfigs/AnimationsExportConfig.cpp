#include "AnimationsExportConfig.hpp"
#include "EditorConfiguration.hpp"


#include <fstream>

#include <Imgui/ImGui.h>

#include <FileUtils/FileSystemHelpers.hpp>
#include <AssetFiles/RawFileFilter.hpp>
#include <FileUtils/AssetFiles/Folder.hpp>
#include <FileUtils/AssetFiles/RawFile.hpp>

namespace AGE
{
	AnimationExportConfigManager *AnimationExportConfigManager::getInstance()
	{
		static AnimationExportConfigManager instance;
		return &instance;
	}

	void AnimationExportConfigManager::update()
	{
		if (_config == nullptr)
			return;
		bool save = false;
		bool close = false;
		bool cook = false;
		_config->displayGui(save, close, cook);

		if (save)
		{
			std::string filepath;
			if (_config->editableName == false)
			{
				filepath = _filePath;
			}
			else
			{
				if (_config->name.empty())
					return;
				filepath = _rootPath + "/" + _config->name + ".AnimExportConfig";
			}
			std::ofstream file(filepath.c_str(), std::ios::binary);
			assert(file.is_open());
			{
				auto ar = cereal::JSONOutputArchive(file);
				ar(*_config.get());
			}
			file.close();
			_config->editableName = false;
		}

		if (close)
			_config = nullptr;
	}

	void AnimationExportConfigManager::create(const std::string &rootPath)
	{
		if (_config)
			return;
		_config = std::make_shared<AnimationsExportConfig>();
		_rootPath = rootPath;
	}

	void AnimationExportConfigManager::edit(const std::string &filePath)
	{
		if (_config)
			return;
		_config = std::make_shared<AnimationsExportConfig>();
		_filePath = filePath;
		_filePath += ".AnimExportConfig";
		std::ifstream file(_filePath.c_str(), std::ios::binary);
		assert(file.is_open());
		{
			auto ar = cereal::JSONInputArchive(file);
			ar(*_config.get());
		}
		file.close();
		_config->editableName = false;
	}


	AnimationsExportConfig::AnimationNode::AnimationNode()
	{
	}

	AnimationsExportConfig::AnimationsExportConfig()
	{

	}

	std::string selectFbx()
	{
		static bool folderInitialized = false;
		static FileUtils::Folder meshFolder = FileUtils::Folder();

		bool hasChanged = false;

		if (folderInitialized == false)
		{
			RawFileFilter filter;
			std::string assetsDirectory = WE::EditorConfiguration::GetRawDirectory();
			meshFolder.list(&filter, assetsDirectory);
			folderInitialized = true;
		}

		std::list<std::string> fbxInFolder;

		fbxInFolder.clear();

		meshFolder.update(
			std::function<bool(FileUtils::Folder*)>([&](FileUtils::Folder* folder) {
			return true;
		}),
			std::function<bool(FileUtils::Folder*)>([](FileUtils::Folder* folder) {
			return true;
		}),
			std::function<void(FileUtils::RawFile*)>([&](FileUtils::RawFile* file) {

			auto extension = FileUtils::GetExtension(file->getFileName());
			// Name of the file without extension and without _static or _dynamic
			std::string phageName;

			if (extension == "fbx")
			{
				phageName = FileUtils::RemoveExtension(file->getFileName());

				if (std::find(fbxInFolder.begin(), fbxInFolder.end(), phageName) == fbxInFolder.end())
				{
					fbxInFolder.push_back(phageName);
				}
			}
		}));

		std::string res = "";
		bool clicked = false;
		ImGui::OpenPopup("Select mesh");
		if (ImGui::BeginPopupModal("Select mesh", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static ImGuiTextFilter filter;

			filter.Draw();
			for (auto &e : fbxInFolder)
			{
				if (filter.PassFilter(e.c_str()))
				{
					if (ImGui::SmallButton(e.c_str()))
					{
						res = e;
						clicked = true;
					}
				}
			}
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); res = "NULL"; }
			if (clicked == true) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}

		return res;
	}

	void AnimationsExportConfig::displayGui(bool &save, bool &close, bool &cook)
	{
		static AnimationNode *selectedNode = nullptr;

		if (selectedNode != nullptr)
		{
			selectedNode->rawAnimationPath = selectFbx();
			if (selectedNode->rawAnimationPath == "NULL")
			{
				selectedNode->rawAnimationPath = "";
			}
			else if (selectedNode->rawAnimationPath.empty() == false)
			{
				selectedNode = nullptr;
			}
			return;
		}

		ImGui::Begin("Animation export config");

		if (editableName)
		{
			static char nameBuf[maxNameLength] = "";
			strcpy(nameBuf, name.c_str());
			if (ImGui::InputText("Config name", nameBuf, maxNameLength))
			{
				name = nameBuf;
			}
		}
		else
		{
			ImGui::TextDisabled("Editing : %s.", name.c_str());
		}

		std::list<AnimationNode>::iterator it = std::begin(animations);
		int id = 0;
		while (it != std::end(animations))
		{
			ImGui::PushID(id);

			bool increment = true;

			static char animnameBuf[maxNameLength] = "";
			strcpy(animnameBuf, it->animationName.c_str());
			if (ImGui::InputText("Animation name", animnameBuf, maxNameLength))
			{
				it->animationName = animnameBuf;
			}
			if (it->rawAnimationPath.empty())
			{
				if (ImGui::Button("Select mesh"))
				{
					selectedNode = &(*it);
				}
			}
			else
			{
				ImGui::Text(it->rawAnimationPath.c_str());
				ImGui::SameLine();
				if (ImGui::Button("Edit"))
				{
					selectedNode = &(*it);
				}
			}

			if (ImGui::Button("Delete"))
			{
				it = animations.erase(it);
				increment = false;
			}
			ImGui::Separator();
			if (increment)
				++it;
			ImGui::PopID();
			++id;
		}
		if (ImGui::Button("Add animation"))
		{
			animations.push_back(AnimationNode());
		}
		if (ImGui::Button("Save and cook"))
		{
			save = true;
			cook = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Save and close"))
		{
			save = true;
			close = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			close = true;
		}
		ImGui::End();
	}
}