#include <AssetFileCreator.hpp>
#include <AssetFiles/UnknownFile.hpp>
#include <AssetFiles/RawMeshFile.hpp>
#include <AssetFiles/RawTextureFile.hpp>
#include <AssetFiles/RawMaterialFile.hpp>

namespace AGE
{
	namespace AE
	{
		std::shared_ptr<AssetFile> AssetFileCreator::CreateFile(const std::tr2::sys::path &path, Folder *parent)
		{
			auto extension = AssetFile::getExtension(path);
			if (extension.empty())
				return std::make_shared<UnknownFile>(path, parent);
			if (extension == "obj" || extension == "fbx")
				return std::make_shared<RawMeshFile>(path, parent);
			if (extension == "bmp" || extension == "jpg" || extension == "jpeg" || extension == "tga" || extension == "png")
				return std::make_shared<RawTextureFile>(path, parent);
			if (extension == "mtl")
				return std::make_shared<RawMaterialFile>(path, parent);
			return std::make_shared<UnknownFile>(path, parent);
		}

		std::shared_ptr<AssetFile> AssetFileCreator::CreateFile(const std::string &path, Folder *parent)
		{
			return CreateFile(std::tr2::sys::path(path), parent);
		}
	}
}
