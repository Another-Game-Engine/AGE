#include "TextureRawFile.hpp"

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
		TextureRawFile::~TextureRawFile()
		{
		}

		TextureRawFile::TextureRawFile(const std::tr2::sys::path &path, Folder *parent)
			: RawFile(path, parent)
		{
		}

		void TextureRawFile::cook()
		{
			dataSet->loadTextures = true;
			auto cookingTask = std::make_shared<CookingTask>(dataSet);
			AGE::EmplaceTask<AGE::Tasks::Basic::VoidFunction>([=]()
			{
				cookingTask->texturesPath.insert(getFileName());
				cookingTask->serializedDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(WE::EditorConfiguration::GetCookedDirectory());
				cookingTask->rawDirectory = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(WE::EditorConfiguration::GetRawDirectory());				
				AGE::ImageLoader::load(cookingTask);
				AGE::ImageLoader::save(cookingTask);
			});
		}

		void TextureRawFile::selection()
		{
			ImGui::Text("Selected asset : "); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.58, 0, 0.7, 1), getFileName().c_str());
			ImGui::Text("Last modifiction : %s", _lastWriteTimeStr.c_str());
			auto dataset = dataSet;
			ImGui::Checkbox("Compress textures", &dataset->compressTextures);
			if (dataset->compressTextures)
			{
				ImGui::SliderInt("Compression quality", &dataset->textureCompressionQuality, 0, 4);
				ImGui::Checkbox("Generate mipmaps", &dataset->generateMipmap);
			}
		}

	}

}