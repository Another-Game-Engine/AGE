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
#include <AssetFiles/CookedFile.hpp>

#include <AssetFiles/AssetsTypes.hpp>

#include <CookConfigs.hpp>

#include <Utils/FileSystem.hpp>


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
		ImGui::BeginChild("Assets browser", ImVec2(ImGui::GetWindowWidth() * 0.3333333f, ImGui::GetIO().DisplaySize.y), true);
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
		ImGui::BeginChild("Selected Raw", ImVec2(ImGui::GetWindowWidth() * 0.33333333f, ImGui::GetIO().DisplaySize.y), false);
		{
			if (_selectedRaw != nullptr)
			{
				ImGui::BeginChild(_selectedRaw->getFileName().c_str(), ImVec2(0, ImGui::GetIO().DisplaySize.y - 10), true);
				ImGui::Text("Selected asset : "); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.58, 0, 0.7, 1), _selectedRaw->getFileName().c_str());
				ImGui::Text("Last modifiction : %s", _selectedRaw->_lastWriteTimeStr.c_str());
				ImGui::Text("Add cook configurations : ");

				std::shared_ptr<AE::CookConfig> newConfig = nullptr;

				if (_selectedRaw->_type & AE::AssetType::Mesh && ImGui::Button("Animation"))
				{
					newConfig = std::make_shared<AE::AnimationConfig>();
				} ImGui::SameLine();
				if (_selectedRaw->_type & AE::AssetType::Mesh && ImGui::Button("Material"))
				{
					newConfig = std::make_shared<AE::MaterialConfig>();
				} ImGui::SameLine();
				if (_selectedRaw->_type & AE::AssetType::Mesh && ImGui::Button("Skin"))
				{
					newConfig = std::make_shared<AE::SkinConfig>();
				} ImGui::SameLine();
				if (_selectedRaw->_type & AE::AssetType::Mesh && ImGui::Button("Physic"))
				{
					newConfig = std::make_shared<AE::PhysicConfig>();
				} ImGui::SameLine();
				if (_selectedRaw->_type & AE::AssetType::Mesh && ImGui::Button("Skeleton"))
				{
					newConfig = std::make_shared<AE::SkeletonConfig>();
				} ImGui::SameLine();
				if (_selectedRaw->_type & AE::AssetType::Texture && ImGui::Button("Texture"))
				{
					newConfig = std::make_shared<AE::TextureConfig>();
				}

				ImGui::Separator();
				ImGui::Separator();

				if (newConfig != nullptr)
				{
					newConfig->rawFile = _selectedRaw;
					_configs.insert(newConfig);
					newConfig->updateLastTimeEdited();
					_selectedRaw->configs.insert(newConfig);
				}
				std::list<std::shared_ptr<AE::CookConfig>> toDelete;
				for (auto &e : _selectedRaw->configs)
				{
					if (ImGui::TreeNode(e.get(), "%s | %s", e->cookConfigTypeToString(), e->name))
					{
						e->update();
						if (ImGui::Button("Delete"))
						{
							toDelete.push_back(e);
						}
						ImGui::TreePop();
					}
				}
				for (auto &e : toDelete)
				{
					_selectedRaw->configs.erase(e);
					_configs.erase(e);
				}
				ImGui::EndChild();
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Todo", ImVec2(ImGui::GetWindowWidth() * 0.33333333f, ImGui::GetIO().DisplaySize.y), false);
		{
			if (ImGui::Button("Cook dirty assets"))
			{
				for (auto &e : _configs)
				{
					if ((e->rawFile.lock() && e->rawFile.lock()->_dirty)
						/*|| (e->cookedFile.lock() && FileSystem::GetDiffTime(e->cookedFile.lock()->_lastWriteTime , e->lastTimeEdited) < 0)*/)
					{
						switch (e->type)
						{
						case(AE::CookConfig::CookConfigType::Skin) :
							//TO FUCKING DO !
							break;
						default:
							break;
						}
					}
				}
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
				AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
					std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
					dataSet->filePath = File("catwoman/catwoman.fbx");
					dataSet->skeletonName = "catwoman";
					dataSet->animationName = "catwoman-roulade";
					dataSet->skinName = "catwoman";
					dataSet->materialName = "catwoman";

					dataSet->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
					dataSet->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

					AGE::AssimpLoader::Load(*dataSet.get());

					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::MaterialLoader::load(*dataSet.get());
						AGE::ImageLoader::load(*dataSet.get());
						AGE::SkeletonLoader::load(*dataSet.get());
						AGE::AnimationsLoader::load(*dataSet.get());
						AGE::MeshLoader::load(*dataSet.get());
						AGE::MaterialLoader::save(*dataSet.get());
						AGE::ImageLoader::save(*dataSet.get());
						AGE::MeshLoader::save(*dataSet.get());
						AGE::SkeletonLoader::save(*dataSet.get());
						AGE::AnimationsLoader::save(*dataSet.get());
						AGE::BulletLoader::load(*dataSet.get());
						AGE::BulletLoader::save(*dataSet.get());
					});
				});
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