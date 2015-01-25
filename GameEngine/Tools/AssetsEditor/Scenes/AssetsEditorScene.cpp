#include "AssetsEditorScene.hpp"

//CORE
#include <Threads/ThreadManager.hpp>
#include <Core/Engine.hh>
#include <Core/Timer.hh>
#include <Core/AssetsManager.hpp>
#include <Threads/RenderThread.hpp>
#include <Utils/Age_Imgui.hpp>
#include <Core/Tasks/Basics.hpp>
#include <Threads/TaskScheduler.hpp>
#include <Core/Input.hh>
#include <SDL/SDL.h>

#include <string>

//CONVERTOR
#include <Convertor/SkeletonLoader.hpp>
#include <Convertor/AnimationsLoader.hpp>
#include <Convertor/MeshLoader.hpp>
#include <Convertor/MaterialConvertor.hpp>
#include <Convertor/ImageLoader.hpp>
#include <Convertor/BulletLoader.hpp>

#include <AssetFiles/Folder.hpp>
#include <AssetFiles/RawFile.hpp>
#include <AssetFiles/AssetFileManager.hpp>


namespace AGE
{
	AssetsEditorScene::AssetsEditorScene(std::weak_ptr<AGE::Engine> engine)
		: AScene(engine)
		, _raw("../../Assets/AGE-Assets-For-Test/Raw")
		, _cook("../../Assets/AGE-Assets-For-Test/Serialized")
	{
		_raw.list();
		_cook.list();
		AE::AssetFileManager::LinkRawToCooked(&_raw, &_cook);
	}

	AssetsEditorScene::~AssetsEditorScene(void)
	{
	}

	bool AssetsEditorScene::userStart()
	{
		return true;
	}

	bool AssetsEditorScene::userUpdate(double time)
	{
		std::shared_ptr<AE::AssetFile> testFind;
		_raw.find("../../Assets/AGE-Assets-For-Test/Raw/ball/ball.obj", testFind);
		//check dirty for test
		{
			static double counter = 0;
			counter += time;
			if (counter > 1)
			{
				std::set<std::shared_ptr<AE::RawFile>> dirty;
				AE::AssetFileManager::CheckIfRawModified(&_raw, dirty);
				if (dirty.size())
					std::cout << "Dirty : " << dirty.size() << std::endl;
				counter = 0;
			}
		}

		ImGui::Begin("Assets Convertor", (bool*)1, ImGui::GetIO().DisplaySize, 1, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
		ImGui::BeginChild("Assets browser", ImVec2(ImGui::GetWindowWidth() * 0.3f, ImGui::GetIO().DisplaySize.y), true);
		{
			{
				ImGui::BeginChild("Raw", ImVec2(0, ImGui::GetIO().DisplaySize.y /*/ 2 */- 10), false);
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
					AE::AssetFileManager::PrintSelectableRawAssetsFile(file, AE::AssetFileManager::PrintSection::Date | AE::AssetFileManager::PrintSection::Name | AE::AssetFileManager::PrintSection::Type, &_selectedRaw);
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
		ImGui::BeginChild("Cook list", ImVec2(ImGui::GetWindowWidth() * 0.15f, ImGui::GetIO().DisplaySize.y), false);
		{
			{
				ImGui::BeginChild("Selected", ImVec2(0, ImGui::GetIO().DisplaySize.y * 0.75 - 10), false);
				for (auto &e : _selectedRaw)
				{
					e->update(std::function<void(AE::RawFile*)>([&](AE::RawFile* file) {
						AE::AssetFileManager::PrintSelectableRawAssetsFile(file, AE::AssetFileManager::PrintSection::Name | AE::AssetFileManager::PrintSection::Type, nullptr);
					}));
				}
				ImGui::EndChild();
			}
			{
				ImGui::BeginChild("Cooked", ImVec2(0, ImGui::GetIO().DisplaySize.y * 0.25 - 10), true);
				//_cook.printImgUi(AE::AssetFile::PrintInfos(AE::AssetFile::Name | AE::AssetFile::Type | AE::AssetFile::Date));
				ImGui::EndChild();
			}
		}
		ImGui::EndChild();
		ImGui::End();

		if (getInstance<Input>()->getInput(SDLK_ESCAPE))
			return (false);

		static bool slowTouch = false;
		if (getInstance<Input>()->getInput(SDLK_e))
		{
			if (!slowTouch)
			{
				slowTouch = true;
				// CATWOMAN
//				AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
//					std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
//					dataSet->filePath = File("catwoman/catwoman.fbx");
//					dataSet->skeletonName = "catwoman";
//					dataSet->animationName = "catwoman-roulade";
//					dataSet->skinName = "catwoman";
//					dataSet->materialName = "catwoman";
//
//					dataSet->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
//					dataSet->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");
//
//					AGE::AssimpLoader::Load(*dataSet.get());
//
//					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
//						AGE::MaterialLoader::load(*dataSet.get());
//						AGE::ImageLoader::load(*dataSet.get());
//						AGE::SkeletonLoader::load(*dataSet.get());
//						AGE::AnimationsLoader::load(*dataSet.get());
//						AGE::MeshLoader::load(*dataSet.get());
//						AGE::MaterialLoader::save(*dataSet.get());
//						AGE::ImageLoader::save(*dataSet.get());
//						AGE::MeshLoader::save(*dataSet.get());
//						AGE::SkeletonLoader::save(*dataSet.get());
//						AGE::AnimationsLoader::save(*dataSet.get());
//						AGE::BulletLoader::load(*dataSet.get());
//						AGE::BulletLoader::save(*dataSet.get());
//					});
//				});
				////SPONZA
				AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
					std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
					dataSet->filePath = File("sponza/sponza.obj");
					dataSet->skinName = "sponza";
					dataSet->materialName = "sponza";

					dataSet->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
					dataSet->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

					AGE::AssimpLoader::Load(*dataSet.get());
					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::MaterialLoader::load(*dataSet.get());
						AGE::MaterialLoader::save(*dataSet.get());

						AGE::ImageLoader::load(*dataSet.get());
						AGE::ImageLoader::save(*dataSet.get());

						AGE::MeshLoader::load(*dataSet.get());
						AGE::MeshLoader::save(*dataSet.get());

						AGE::BulletLoader::load(*dataSet.get());
						AGE::BulletLoader::save(*dataSet.get());
					});
				});
				//CUBE
				AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
					std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
					dataSet->filePath = File("cube/cube.obj");
					dataSet->skinName = "cube";
					dataSet->materialName = "cube";

					dataSet->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
					dataSet->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

					AGE::AssimpLoader::Load(*dataSet.get());

					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::MaterialLoader::load(*dataSet.get());
						AGE::MaterialLoader::save(*dataSet.get());

						AGE::ImageLoader::load(*dataSet.get());
						AGE::ImageLoader::save(*dataSet.get());

						AGE::MeshLoader::load(*dataSet.get());
						AGE::MeshLoader::save(*dataSet.get());

						AGE::BulletLoader::load(*dataSet.get());
						AGE::BulletLoader::save(*dataSet.get());
					});
				});
				//BALL
				AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
					std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
					dataSet->filePath = File("ball/ball.obj");
					dataSet->skinName = "ball";
					dataSet->materialName = "ball";
					dataSet->physicName = "ball";

					dataSet->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
					dataSet->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

					AGE::AssimpLoader::Load(*dataSet.get());

					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::MaterialLoader::load(*dataSet.get());
						AGE::MaterialLoader::save(*dataSet.get());

						AGE::ImageLoader::load(*dataSet.get());
						AGE::ImageLoader::save(*dataSet.get());

						AGE::MeshLoader::load(*dataSet.get());
						AGE::MeshLoader::save(*dataSet.get());

						AGE::BulletLoader::load(*dataSet.get());
						AGE::BulletLoader::save(*dataSet.get());
					});
				});
			}
			slowTouch = true;
		}
		else
			slowTouch = false;
		return true;
	}
}