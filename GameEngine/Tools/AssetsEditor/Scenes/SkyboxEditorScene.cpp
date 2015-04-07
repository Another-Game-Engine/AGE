#include "SkyboxEditorScene.hh"

//CORE
#include <Threads/ThreadManager.hpp>
#include <Core/Engine.hh>
#include <Core/Timer.hh>
#include <AssetManagement/AssetManager.hh>
#include <Threads/RenderThread.hpp>
#include <Utils/Age_Imgui.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Threads/TaskScheduler.hpp>

#include <string>

//CONVERTOR
#include <Convertor/SkeletonLoader.hpp>
#include <Convertor/AnimationsLoader.hpp>
#include <Convertor/MeshLoader.hpp>
#include <Convertor/MaterialConvertor.hpp>
#include <Convertor/ImageLoader.hpp>
#include <Convertor/BulletLoader.hpp>
#include <Convertor/ConvertorStatusManager.hpp>

#include <AssetFiles/Folder.hpp>
#include <AssetFiles/RawFile.hpp>
#include <AssetFiles/AssetFileManager.hpp>
#include <AssetFiles/CookedFile.hpp>
#include <AssetFiles/AssetsTypes.hpp>

#include <Utils/FileSystem.hpp>
#include <Utils/Directory.hpp>
#include <Utils/Path.hpp>
#include <Utils/Debug.hpp>
#include <Utils/FileSystemHelpers.hpp>

#include <EditorConfiguration.hpp>

namespace AGE
{
	const std::string SkyboxEditorScene::Name = "SkyboxEditor";
	
	SkyboxEditorScene::SkyboxEditorScene(AGE::Engine *engine)
		: AScene(engine)
	{
	}

	SkyboxEditorScene::~SkyboxEditorScene(void)
	{
	}

	bool SkyboxEditorScene::_userStart()
	{
		Singleton<AGE::AE::ConvertorStatusManager>::setInstance();
		return true;
	}

	bool SkyboxEditorScene::_userUpdateBegin(float time)
	{
		const std::string currentDir = Directory::GetCurrentDirectory();
		const std::string absPath = Path::AbsoluteName(currentDir.c_str(), WE::EditorConfiguration::GetCookedDirectory().c_str());
		Directory dir;
		const bool succeed = dir.open(absPath.c_str());
		AGE_ASSERT(succeed && "Impossible to open directory");
		for (auto it = dir.recursive_begin(); it != dir.recursive_end(); ++it)
		{
			if (Directory::IsFile(*it))
			{
				auto extension = AGE::FileSystemHelpers::GetExtension(*it);
				if (extension == "tage")
				{
				}
			}
		}
		dir.close();
		ImGui::BeginChild("Assets browser", ImVec2(ImGui::GetWindowWidth() * 0.333333f, 0), true);
		{
			{
				ImGui::BeginChild("Raw", ImVec2(0, 0), false);
				//_raw.update(
				//	std::function<bool(AE::Folder*)>([](AE::Folder* folder) {
				//	bool opened = ImGui::TreeNode((void*)(folder), folder->_path.path().filename().c_str());
				//	if (opened)
				//	{
				//		ImGui::Separator();
				//	}
				//	return opened;
				//}),
				//	std::function<bool(AE::Folder*)>([](AE::Folder* folder) {
				//	ImGui::Separator();
				//	ImGui::TreePop();
				//	return true;
				//}),
				//	std::function<void(AE::RawFile*)>([&](AE::RawFile* file) {
				//	AE::AssetFileManager::PrintClickableRawAssetsFile(file, AE::AssetFileManager::PrintSection::Date | AE::AssetFileManager::PrintSection::Name | AE::AssetFileManager::PrintSection::Type, _selectedRaw);
				//}));
				ImGui::EndChild();
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Selected Raw", ImVec2(ImGui::GetWindowWidth() * 0.33333333f, 0), false);
		{
			//if (_selectedRaw != nullptr)
			//{
			//	if (_selectedRaw->dataSet == nullptr)
			//	{
			//		auto path = _selectedRaw->getPath();
			//		path = path.erase(0, _raw._path.path().string().size());
			//		//_selectedRaw->dataSet->filePath = File(path);
			//		_selectedRaw->dataSet = std::make_shared<AssetDataSet>(path);
			//
			//	}
			//	if (!_selectedRaw->dataSet->isConverting)
			//	{
			//		_selectedRaw->selection();
			//		ImGui::Separator();
			//		if (ImGui::Button("Cook"))
			//			_selectedRaw->cook();
			//	}
			//	else
			//	{
			//		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Currently cooking : %s", _selectedRaw->getFileName().c_str());
			//	}
			//}
		}
		ImGui::EndChild();
		return true;
	}

	bool SkyboxEditorScene::_userUpdateEnd(float time)
	{
		ImGui::End();
		return true;
	}
}