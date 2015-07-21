#include "MeshRawFile.hpp"

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
#include <Convertor/PhysicsLoader.hpp>
#include <Convertor/ConvertorStatusManager.hpp>

#include <EditorConfiguration.hpp>

namespace AGE
{
	namespace AE
	{
		MeshRawFile::~MeshRawFile()
		{
		}

		MeshRawFile::MeshRawFile(const std::tr2::sys::path &path, Folder *parent)
			: RawFile(path, parent)
		{
		}

		void MeshRawFile::cook()
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
					AGE::PhysicsLoader::load(cookingTask);
					AGE::PhysicsLoader::save(cookingTask);
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
						AGE::PhysicsLoader::load(cookingTask);
						AGE::PhysicsLoader::save(cookingTask);
					});
					AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=](){
						AGE::SkeletonLoader::load(cookingTask);
						AGE::SkeletonLoader::save(cookingTask);
					});
				}
			});
		}

		void MeshRawFile::selection()
		{
			ImGui::Text("Selected asset : "); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.58f, 0.0f, 0.7f, 1.0f), getFileName().c_str());
			ImGui::Text("Last modifiction : %s", _lastWriteTimeStr.c_str());

			auto dataset = dataSet;

			ImGui::Checkbox("Mesh", &dataset->loadMesh);
			if (dataset->loadMesh)
			{
				ImGui::Checkbox("Normalize size", &dataset->normalize);
				if (dataset->normalize)
				{
					ImGui::SliderFloat("Max size length", &dataset->maxSideLength, 0.00001f, 1000.0f, "%.3f", 1.0f);
				}
				ImGui::Checkbox("Positions", &dataset->positions);
				ImGui::Checkbox("Normals", &dataset->normals);
				ImGui::Checkbox("Bones infos", &dataset->bonesInfos);
				ImGui::Checkbox("Texture coordinates", &dataset->uvs);
				ImGui::Checkbox("Tangents", &dataset->tangents);
				ImGui::Checkbox("BiTangents", &dataset->biTangents);
			}
			ImGui::Separator();

			ImGui::Checkbox("Physic", &dataset->loadPhysic);
			if (dataset->loadPhysic)
			{
				ImGui::Checkbox("Convex", &dataset->convex);
				ImGui::Checkbox("Concave", &dataset->concave);
			}
			ImGui::Separator();

			ImGui::Checkbox("Material", &dataset->loadMaterials);
			if (dataset->loadMaterials)
			{
				ImGui::Checkbox("Generate normal map from bump", &dataset->bumpToNormal);
				if (dataset->bumpToNormal)
				{
					ImGui::SliderFloat("Normal strength", &dataset->normalStrength, 1.0f, 10.0f);
				}
			}
			ImGui::Separator();

			ImGui::Checkbox("Animation", &dataset->loadAnimations);
			if (dataset->loadAnimations)
			{
			}
			ImGui::Separator();

			ImGui::Checkbox("Skeleton", &dataset->loadSkeleton);
			if (dataset->loadSkeleton || dataset->loadMesh || dataset->loadAnimations)
			{
			}
			ImGui::Separator();

			ImGui::Checkbox("Textures", &dataset->loadTextures);
			if (dataset->loadTextures)
			{
				ImGui::Checkbox("Compress textures", &dataset->compressTextures);
				if (dataset->compressTextures)
				{
					ImGui::SliderInt("Compression quality", &dataset->textureCompressionQuality, 0, 4);
					ImGui::Checkbox("Generate mipmaps", &dataset->generateMipmap);
				}
			}
		}
	
	}
	
}