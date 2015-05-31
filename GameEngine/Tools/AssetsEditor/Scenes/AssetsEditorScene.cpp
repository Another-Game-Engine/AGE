#include "AssetsEditorScene.hpp"

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
	const std::string AssetsEditorScene::Name = "Assets Editor";
	AE::Folder AssetsEditorScene::_raw = AE::Folder();
	AE::Folder AssetsEditorScene::_cook = AE::Folder();

	std::vector<AssetsEditorScene::AssetsEditorFileDescriptor> AssetsEditorScene::_cookedFiles
		= std::vector<AssetsEditorScene::AssetsEditorFileDescriptor>();

	std::vector<const char *> AssetsEditorScene::_cookedMeshsFullPath = std::vector<const char *>();
	std::vector<const char *> AssetsEditorScene::_cookedMeshFiles = std::vector<const char *>();

	std::vector<const char *> AssetsEditorScene::_cookedBulletFullPath = std::vector<const char *>();
	std::vector<const char *> AssetsEditorScene::_cookedBulletFiles = std::vector<const char *>();

	std::vector<const char *> AssetsEditorScene::_cookedMaterialFullPath = std::vector<const char *>();
	std::vector<const char *> AssetsEditorScene::_cookedMaterialFiles = std::vector<const char *>();

	std::vector<const char *> AssetsEditorScene::_cookedTextureFullPath = std::vector<const char *>();
	std::vector<const char *> AssetsEditorScene::_cookedTextureFiles = std::vector<const char *>();

	AssetsEditorScene::AssetsEditorScene(AGE::Engine *engine)
		: AScene(engine)
	{
		_raw.list(WE::EditorConfiguration::GetRawDirectory());
		AE::AssetFileManager::LinkRawToCooked(&_raw, &_cook);
	}

	AssetsEditorScene::~AssetsEditorScene(void)
	{
	}

	bool AssetsEditorScene::_userStart()
	{
		Singleton<AGE::AE::ConvertorStatusManager>::setInstance();
		return true;
	}

	bool AssetsEditorScene::_userUpdateBegin(float time)
	{
		// dirty ! We list files
		static float refreshCounter = 1.0f;
		if (refreshCounter >= 1.0f)
		{
			_cookedBulletFiles.clear();
			_cookedBulletFullPath.clear();
			_cookedMaterialFiles.clear();
			_cookedMaterialFullPath.clear();
			_cookedMeshFiles.clear();
			_cookedMeshsFullPath.clear();
			_cookedFiles.clear();
			_cookedTextureFiles.clear();
			_cookedTextureFullPath.clear();

			_cookedBulletFiles.push_back("NONE");
			_cookedBulletFullPath.push_back("NONE");
			_cookedMaterialFiles.push_back("NONE");
			_cookedMaterialFullPath.push_back("NONE");
			_cookedMeshFiles.push_back("NONE");
			_cookedMeshsFullPath.push_back("NONE");
			_cookedTextureFiles.push_back("NONE");
			_cookedTextureFullPath.push_back("NONE");

			const std::string currentDir = Directory::GetCurrentDirectory();
			const std::string absPath = Path::AbsoluteName(currentDir.c_str(), WE::EditorConfiguration::GetCookedDirectory().c_str());
			Directory dir;
			const bool succeed = dir.open(absPath.c_str());
			AGE_ASSERT(succeed && "Impossible to open directory");
			for (auto it = dir.recursive_begin(); it != dir.recursive_end(); ++it)
			{
				if (Directory::IsFile(*it))
				{
					_cookedFiles.push_back(AssetsEditorFileDescriptor(Path::RelativeName(absPath.c_str(), *it), Path::BaseName(*it)));

					auto extension = AGE::FileSystemHelpers::GetExtension(*it);
					if (extension == "sage")
					{
						_cookedMeshFiles.push_back(_cookedFiles.back().fileName.c_str());
						_cookedMeshsFullPath.push_back(_cookedFiles.back().fullPath.c_str());
					}
					else if (extension == "mage")
					{
						_cookedMaterialFiles.push_back(_cookedFiles.back().fileName.c_str());
						_cookedMaterialFullPath.push_back(_cookedFiles.back().fullPath.c_str());
					}
					else if (extension == "phage")
					{
						_cookedBulletFiles.push_back(_cookedFiles.back().fileName.c_str());
						_cookedBulletFullPath.push_back(_cookedFiles.back().fullPath.c_str());
					}
					else if (extension == "tage")
					{
						_cookedTextureFiles.push_back(_cookedFiles.back().fileName.c_str());
						_cookedTextureFullPath.push_back(_cookedFiles.back().fullPath.c_str());
					}
				}
			}
			dir.close();
			refreshCounter = 0;
		}
		refreshCounter += time;

		ImGui::BeginChild("Assets browser", ImVec2(ImGui::GetWindowWidth() * 0.333333f, 0), true);
		{
			{
				ImGui::BeginChild("Raw", ImVec2(0, 0), false);
				_raw.update(
				std::function<bool(AE::Folder*)>([](AE::Folder* folder) {
					bool opened = ImGui::TreeNode((void*)(folder), folder->_path.path().filename().c_str());
					if (opened)
					{
						ImGui::Separator();
					}
					return opened;
				}),
					std::function<bool(AE::Folder*)>([](AE::Folder* folder) {
					ImGui::Separator();
					ImGui::TreePop();
					return true;
				}),
					std::function<void(AE::RawFile*)>([&](AE::RawFile* file) {
					AE::AssetFileManager::PrintClickableRawAssetsFile(file, AE::AssetFileManager::PrintSection::Date | AE::AssetFileManager::PrintSection::Name | AE::AssetFileManager::PrintSection::Type, _selectedRaw);
				}));
				ImGui::EndChild();
			}
			//{
			//	ImGui::BeginChild("Cooked", ImVec2(0, ImGui::GetIO().DisplaySize.y / 2 - 10), false);
			//	//_cook.printImgUi(AE::AssetFile::PrintInfos(AE::AssetFile::Name | AE::AssetFile::Type | AE::AssetFile::Date));
			//	ImGui::EndChild();
			//}
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Selected Raw", ImVec2(ImGui::GetWindowWidth() * 0.33333333f, 0), false);
		{
			if (_selectedRaw != nullptr)
			{
				if (_selectedRaw->dataSet == nullptr)
				{
					auto path = _selectedRaw->getPath();
					path = path.erase(0, _raw._path.path().string().size());
					//_selectedRaw->dataSet->filePath = File(path);
					_selectedRaw->dataSet = std::make_shared<AssetDataSet>(path);

				}
				if (!_selectedRaw->dataSet->isConverting)
				{
					_selectedRaw->selection();
					ImGui::Separator();
					if (ImGui::Button("Cook"))
						_selectedRaw->cook();
				}
				else
				{
					ImGui::TextColored(ImVec4(1, 0, 0, 1), "Currently cooking : %s", _selectedRaw->getFileName().c_str());
				}
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Todo", ImVec2(ImGui::GetWindowWidth() * 0.33333333f, 0), false);
		Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->DisplayTasks();
		ImGui::EndChild();
		return true;
	}

	bool AssetsEditorScene::_userUpdateEnd(float time)
	{
		ImGui::End();
		return true;
	}
}