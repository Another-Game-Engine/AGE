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
#include <Convertor/PhysicsLoader.hpp>
#include <Convertor/ConvertorStatusManager.hpp>
#include <Convertor/AssetDataSet.hpp>

#include <FileUtils/AssetFiles/Folder.hpp>
#include <FileUtils/AssetFiles/RawFile.hpp>
#include <FileUtils/AssetFiles/CookedFile.hpp>
#include <FileUtils/AssetFiles/AssetsTypes.hpp>
#include <AssetFiles/AssetFileManager.hpp>

#include <Utils/FileSystem.hpp>
#include <Utils/Directory.hpp>
#include <Utils/Path.hpp>
#include <Utils/Debug.hpp>
#include <FileUtils/FileUtils/FileSystemHelpers.hpp>

#include <EditorConfiguration.hpp>
#include <AssetFiles/RawFileFilter.hpp>

namespace AGE
{
	const std::string AssetsEditorScene::Name = "Assets Editor";
	FileUtils::Folder AssetsEditorScene::_raw = FileUtils::Folder();
	FileUtils::Folder AssetsEditorScene::_cook = FileUtils::Folder();

	std::vector<AssetsEditorScene::AssetsEditorFileDescriptor> AssetsEditorScene::_cookedFiles
		= std::vector<AssetsEditorScene::AssetsEditorFileDescriptor>();

	std::vector<const char *> AssetsEditorScene::_cookedMeshsFullPath = std::vector<const char *>();
	std::vector<const char *> AssetsEditorScene::_cookedMeshFiles = std::vector<const char *>();

	std::vector<const char *> AssetsEditorScene::_cookedPhysicsFullPath = std::vector<const char *>();
	std::vector<const char *> AssetsEditorScene::_cookedPhysicsFiles = std::vector<const char *>();

	std::vector<const char *> AssetsEditorScene::_cookedMaterialFullPath = std::vector<const char *>();
	std::vector<const char *> AssetsEditorScene::_cookedMaterialFiles = std::vector<const char *>();

	std::vector<const char *> AssetsEditorScene::_cookedTextureFullPath = std::vector<const char *>();
	std::vector<const char *> AssetsEditorScene::_cookedTextureFiles = std::vector<const char *>();

	AssetsEditorScene::AssetsEditorScene(AGE::Engine *engine)
		: AScene(engine)
	{
		RawFileFilter filter;
		_raw.list(&filter, WE::EditorConfiguration::GetRawDirectory());
		AE::AssetFileManager::LinkRawToCooked(&_raw, &_cook);
	}

	AssetsEditorScene::~AssetsEditorScene(void)
	{
	}

	void AssetsEditorScene::updateMenu()
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
			_cookedPhysicsFiles.clear();
			_cookedPhysicsFullPath.clear();
			_cookedMaterialFiles.clear();
			_cookedMaterialFullPath.clear();
			_cookedMeshFiles.clear();
			_cookedMeshsFullPath.clear();
			_cookedFiles.clear();
			_cookedTextureFiles.clear();
			_cookedTextureFullPath.clear();

			_cookedPhysicsFiles.push_back("NONE");
			_cookedPhysicsFullPath.push_back("NONE");
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

					auto extension = FileUtils::GetExtension(*it);
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
						_cookedPhysicsFiles.push_back(_cookedFiles.back().fileName.c_str());
						_cookedPhysicsFullPath.push_back(_cookedFiles.back().fullPath.c_str());
					}
					else if (extension == "dds")
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

		if (_displayWindow == false)
		{
			return true;
		}

		ImGui::Begin("Assets browser");
		{
			{
				ImGui::BeginChild("Raw", ImVec2(0, 0), false);
				_raw.update(
					std::function<bool(FileUtils::Folder*)>([](FileUtils::Folder* folder) {
					bool opened = ImGui::TreeNode((void*)(folder), folder->_path.path().filename().c_str());
					if (opened)
					{
						ImGui::Separator();
					}
					return opened;
				}),
					std::function<bool(FileUtils::Folder*)>([](FileUtils::Folder* folder) {
					ImGui::Separator();
					ImGui::TreePop();
					return true;
				}),
					std::function<void(FileUtils::RawFile*)>([&](FileUtils::RawFile* file) {
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
		Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->DisplayTasks();
		ImGui::End();

		if (_selectedRaw != nullptr)
		{
			ImGui::Begin("Selected Raw");
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
			ImGui::End();
		}
		return true;
	}

	bool AssetsEditorScene::_userUpdateEnd(float time)
	{
		return true;
	}
}