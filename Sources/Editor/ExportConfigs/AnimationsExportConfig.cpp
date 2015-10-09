#include "AnimationsExportConfig.hpp"
#include "EditorConfiguration.hpp"


#include <fstream>

#include <Imgui/ImGui.h>

#include <FileUtils/FileSystemHelpers.hpp>
#include <AssetFiles/RawFileFilter.hpp>
#include <FileUtils/AssetFiles/Folder.hpp>
#include <FileUtils/AssetFiles/RawFile.hpp>

//CONVERTOR
#include <Convertor/SkeletonLoader.hpp>
#include <Convertor/AnimationsLoader.hpp>
#include <Convertor/MeshLoader.hpp>
#include <Convertor/MaterialConvertor.hpp>
#include <Convertor/ImageLoader.hpp>
#include <Convertor/PhysicsLoader.hpp>
#include <Convertor/ConvertorStatusManager.hpp>
#include <Convertor/AssetDataSet.hpp>

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
		if (cook)
			this->cook();
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
				phageName = file->getPath();

				fbxInFolder.push_back(phageName);
			}
		}));

		std::string res = "";
		bool clicked = false;
		ImGui::Begin("Select mesh");
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
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { res = "NULL"; }
		ImGui::End();

		return res;
	}

	void AnimationsExportConfig::displayGui(bool &save, bool &close, bool &cook)
	{
		static std::string *selectedNode = nullptr;

		if (selectedNode != nullptr)
		{
			*selectedNode = selectFbx();
			if (*selectedNode == "NULL")
			{
				*selectedNode = "";
				selectedNode = nullptr;
			}
			else if (selectedNode->empty() == false)
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

		if (rawMeshPath.empty())
		{
			if (ImGui::Button("Select mesh for skin and skeleton"))
			{
				selectedNode = &rawMeshPath;
			}
		}
		else
		{
			ImGui::Text(rawMeshPath.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Edit"))
			{
				selectedNode = &rawMeshPath;
			}
		}

		std::list<AnimationNode>::iterator it = std::begin(animations);
		int id = 0;
		while (it != std::end(animations))
		{
			ImGui::PushID(id);

			bool increment = true;

			if (it->rawAnimationPath.empty())
			{
				if (ImGui::Button("Select mesh"))
				{
					selectedNode = &(it->rawAnimationPath);
				}
			}
			else
			{
				ImGui::Text(it->rawAnimationPath.c_str());
				ImGui::SameLine();
				if (ImGui::Button("Edit"))
				{
					selectedNode = &(it->rawAnimationPath);
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
		ImGui::Separator();
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

	void AnimationExportConfigManager::cook()
	{
		auto meshPath = _config->rawMeshPath;
		auto rawDir = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(WE::EditorConfiguration::GetRawDirectory()).path().directory_string();
		std::replace(rawDir.begin(), rawDir.end(), '\\', '/');
		auto find = meshPath.find(rawDir);
		if (find != std::string::npos)
		{
			meshPath.erase(find, find + rawDir.size());
		}
		else
		{
			std::cerr << "Error not finding path blablabla." << std::endl;
			AGE_BREAK();
		}

		auto dataSet = std::make_shared<AssetDataSet>(meshPath);
		auto cookingTask = std::make_shared<CookingTask>(dataSet);

		cookingTask->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(WE::EditorConfiguration::GetCookedDirectory());
		cookingTask->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(WE::EditorConfiguration::GetRawDirectory());

		dataSet->loadSkeleton = true;
		dataSet->loadAnimations = false;
		dataSet->loadMesh = true;
		dataSet->loadMaterials = false;
		dataSet->loadTextures = false;
		dataSet->loadPhysic = false;
		dataSet->normalize = false;

		cookingTask->skeleton = nullptr;
		cookingTask->mesh = nullptr;

		AGE::AssimpLoader::Load(cookingTask);
		AGE::SkeletonLoader::load(cookingTask);
		AGE::MeshLoader::load(cookingTask);

		for (auto &it : _config->animations) // chaque animation
		{
			if (it.rawAnimationPath.empty())
				continue;

			meshPath = it.rawAnimationPath;
			find = meshPath.find(rawDir);
			if (find != std::string::npos)
			{
				meshPath.erase(find, find + rawDir.size());
			}
			else
			{
				std::cerr << "Error not finding path blablabla." << std::endl;
				AGE_BREAK();
			}

			auto dataSetAnim = std::make_shared<AssetDataSet>(meshPath);
			auto cookingTaskAnim = std::make_shared<CookingTask>(dataSetAnim);


			cookingTaskAnim->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(WE::EditorConfiguration::GetCookedDirectory());
			cookingTaskAnim->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(WE::EditorConfiguration::GetRawDirectory());

			dataSetAnim->loadSkeleton = false;
			dataSetAnim->loadAnimations = true;
			dataSetAnim->loadMesh = false;
			dataSetAnim->loadMaterials = false;
			dataSetAnim->loadTextures = false;
			dataSetAnim->loadPhysic = false;
			cookingTaskAnim->skeleton = cookingTask->skeleton;
			cookingTaskAnim->mesh = cookingTask->mesh;
			AGE::AssimpLoader::Load(cookingTaskAnim);
			AGE::AnimationsLoader::load(cookingTaskAnim);
			AGE::AnimationsLoader::save(cookingTaskAnim);
		}
		AGE::SkeletonLoader::save(cookingTask);
		AGE::MeshLoader::save(cookingTask);
	}

}