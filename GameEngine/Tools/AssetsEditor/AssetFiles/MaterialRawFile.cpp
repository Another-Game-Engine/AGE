#include "MaterialRawFile.hpp"

//CORE
#include <Threads/ThreadManager.hpp>
#include <Core/Engine.hh>
#include <Core/Timer.hh>
#include <AssetManagement/AssetManager.hh>
#include <Threads/RenderThread.hpp>
#include <Utils/Age_Imgui.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Threads/TaskScheduler.hpp>


//CONVERTOR
#include <Convertor/SkeletonLoader.hpp>
#include <Convertor/AnimationsLoader.hpp>
#include <Convertor/MeshLoader.hpp>
#include <Convertor/MaterialConvertor.hpp>
#include <Convertor/ImageLoader.hpp>
#include <Convertor/BulletLoader.hpp>
#include <Convertor/ConvertorStatusManager.hpp>

#include <EditorConfiguration.hpp>

namespace AGE
{
	namespace AE
	{
		MaterialRawFile::~MaterialRawFile()
		{
		}

		MaterialRawFile::MaterialRawFile(const std::tr2::sys::path &path, Folder *parent)
			: RawFile(path, parent)
		{
		}

		void MaterialRawFile::cook()
		{
			auto cookingTask = std::make_shared<CookingTask>(dataSet);
			AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=]()
			{
				cookingTask->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(WE::EditorConfiguration::GetCookedDirectory());
				cookingTask->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(WE::EditorConfiguration::GetRawDirectory());

				AGE::AssimpLoader::Load(cookingTask);

				//AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
				AGE::MaterialLoader::load(cookingTask);
				AGE::MaterialLoader::save(cookingTask);
				AGE::ImageLoader::load(cookingTask);
				AGE::ImageLoader::save(cookingTask);
				//});
				if ((cookingTask->dataSet->loadMesh || cookingTask->dataSet->loadAnimations) && cookingTask->dataSet->loadSkeleton)
				{
					AGE::SkeletonLoader::load(cookingTask);
					AGE::AnimationsLoader::load(cookingTask);
					AGE::MeshLoader::load(cookingTask);
					AGE::BulletLoader::load(cookingTask);
					AGE::BulletLoader::save(cookingTask);
					AGE::MeshLoader::save(cookingTask);
					AGE::AnimationsLoader::save(cookingTask);
					AGE::SkeletonLoader::save(cookingTask);

				}
				else
				{
					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::AnimationsLoader::load(cookingTask);
						AGE::AnimationsLoader::save(cookingTask);
					});
					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::MeshLoader::load(cookingTask);
						AGE::MeshLoader::save(cookingTask);
						AGE::BulletLoader::load(cookingTask);
						AGE::BulletLoader::save(cookingTask);
					});
					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::SkeletonLoader::load(cookingTask);
						AGE::SkeletonLoader::save(cookingTask);
					});
				}
			});
		}

		void MaterialRawFile::selection()
		{
			ImGui::Text("Selected asset : "); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.58, 0, 0.7, 1), getFileName().c_str());
			ImGui::Text("Last modifiction : %s", _lastWriteTimeStr.c_str());

			auto dataset = dataSet;

			ImGui::Checkbox("Generate normal map from bump", &dataset->bumpToNormal);
			if (dataset->bumpToNormal)
			{
				ImGui::SliderFloat("Normal strength", &dataset->normalStrength, 1.0f, 10.0f);
			}
		}

	}

}