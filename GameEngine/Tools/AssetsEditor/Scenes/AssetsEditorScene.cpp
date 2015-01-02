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
		if (getInstance<Input>()->getInput(SDLK_e))
		{
			std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
			dataSet->filePath = File("catwoman/catwoman.fbx");
			dataSet->skeletonName = "catwoman";
			dataSet->animationName = "catwoman-roulade";
			dataSet->skinName = "catwoman";
			dataSet->materialName = "catwoman";

			dataSet->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
			dataSet->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

			AGE::AssimpLoader::Load(*dataSet.get());
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
		}
		static bool slowTouch = false;
		if (getInstance<Input>()->getInput(SDLK_a))
		{
			if (!slowTouch)
			{
				slowTouch = true;
				std::cout << "foo" << std::endl;
				std::shared_ptr<AGE::AssetDataSet> dataSet = std::make_shared<AGE::AssetDataSet>();
				dataSet->filePath = File("catwoman/catwoman.fbx");
				dataSet->skeletonName = "catwoman";
				dataSet->animationName = "catwoman-roulade";
				dataSet->skinName = "catwoman";
				dataSet->materialName = "catwoman";

				dataSet->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Serialized");
				dataSet->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>("../../Assets/AGE-Assets-For-Test/Raw");

				AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
					AGE::AssimpLoader::Load(*dataSet.get());
					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::SkeletonLoader::load(*dataSet.get());
						AGE::MeshLoader::load(*dataSet.get());
						AGE::SkeletonLoader::save(*dataSet.get());
						AGE::MeshLoader::save(*dataSet.get());
					});
					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::MaterialLoader::load(*dataSet.get());
						AGE::MaterialLoader::save(*dataSet.get());
					});
					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::ImageLoader::load(*dataSet.get());
						AGE::ImageLoader::save(*dataSet.get());
					});
					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::AnimationsLoader::load(*dataSet.get());
						AGE::AnimationsLoader::save(*dataSet.get());
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