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
	AE::Folder SkyboxEditorScene::_raw = AE::Folder();

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
		int current_item;
		std::vector<std::string> _items;
		for (auto it = dir.recursive_begin(); it != dir.recursive_end(); ++it)
		{
			if (Directory::IsFile(*it))
			{
				auto extension = AGE::FileSystemHelpers::GetExtension(*it);
				//if (extension == "tage")
				//	_items.push_back(AGE::FileSystem::getFileName(*it));
			}
		}
		dir.close();
		char const **tab = new char const *[_items.size()];
		for (auto index = 0; index < _items.size(); ++index) {
			tab[index] = _items[index].c_str();
		}
		if (ImGui::ListBox("Skybox list", &current_item, tab, _items.size())) {

		}
		if (ImGui::Button("Create Skybox")) {

		}
		return true;
	}

	bool SkyboxEditorScene::_userUpdateEnd(float time)
	{
		ImGui::End();
		return true;
	}
}