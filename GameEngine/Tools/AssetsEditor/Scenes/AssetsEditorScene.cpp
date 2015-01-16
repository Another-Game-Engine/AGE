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


//CONVERTOR
#include <Convertor/SkeletonLoader.hpp>
#include <Convertor/AnimationsLoader.hpp>
#include <Convertor/MeshLoader.hpp>
#include <Convertor/MaterialConvertor.hpp>
#include <Convertor/ImageLoader.hpp>

#include <Folder.hpp>


namespace AGE
{
	static AE::Folder raw(std::string("../../Assets/AGE-Assets-For-Test/Raw"));
	static AE::Folder cook(std::string("../../Assets/AGE-Assets-For-Test/Serialized"));
	AssetsEditorScene::AssetsEditorScene(std::weak_ptr<AGE::Engine> engine)
		: AScene(engine)
	{
		raw.list();
		cook.list();
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
		ImGui::Begin("Raw");
		raw.printImgUi(AE::AssetFile::PrintInfos(AE::AssetFile::Name | AE::AssetFile::Type | AE::AssetFile::Date));
		ImGui::End();
		ImGui::Begin("Cooked");
		cook.printImgUi(AE::AssetFile::PrintInfos(AE::AssetFile::Name | AE::AssetFile::Type | AE::AssetFile::Date));
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
					});
				});
				//SPONZA
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
					});
				});
				//BALL
				AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
					std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
					dataSet->filePath = File("ball/ball.obj");
					dataSet->skinName = "ball";
					dataSet->materialName = "ball";

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
					});
				});
				//BROKEN TOWER
				AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
					std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
					dataSet->filePath = File("Broken Tower/Broken Tower.fbx");
					dataSet->skinName = "tower";
					dataSet->materialName = "tower";

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
					});
				});
				//VENICE
				//AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
				//	std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
				//	dataSet->filePath = File("Venice/venice.obj");
				//	dataSet->skinName = "venice";
				//	dataSet->materialName = "venice";

				//	dataSet->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
				//	dataSet->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

				//	AGE::AssimpLoader::Load(*dataSet.get());

				//	AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
				//		AGE::MaterialLoader::load(*dataSet.get());
				//		AGE::MaterialLoader::save(*dataSet.get());

				//		AGE::ImageLoader::load(*dataSet.get());
				//		AGE::ImageLoader::save(*dataSet.get());

				//		AGE::MeshLoader::load(*dataSet.get());
				//		AGE::MeshLoader::save(*dataSet.get());
				//	});
				//});
			}
			slowTouch = true;
		}
		else
			slowTouch = false;
		return true;
	}
}