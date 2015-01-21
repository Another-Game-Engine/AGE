#include <AssetFiles/AssetFileCreator.hpp>
#include <Utils/FileSystem.hpp>
#include <AssetFiles/RawFile.hpp>
#include <AssetFiles/AssetsTypes.hpp>

namespace AGE
{
	namespace AE
	{
		std::shared_ptr<AssetFile> AssetFileCreator::CreateFile(const std::tr2::sys::path &path, Folder *parent)
		{
			auto extension = AGE::FileSystem::GetExtension(path);
			std::shared_ptr<AssetFile> t = nullptr;
			if (extension == "obj" || extension == "fbx" || extension == "collada")
			{
				t = std::make_shared<RawFile>(path, parent);
				t->_type = AssetType::Raw | AssetType::Mesh;
			}
			else if (extension == "bmp" || extension == "jpg" || extension == "jpeg" || extension == "tga" || extension == "png")
			{
				t = std::make_shared<RawFile>(path, parent);
				t->_type = AssetType::Raw | AssetType::Texture;

			}
			else if (extension == "mtl")
			{
				t =  std::make_shared<RawFile>(path, parent);
				t->_type = AssetType::Raw | AssetType::Material;

			}
			return t;
		}

		std::shared_ptr<AssetFile> AssetFileCreator::CreateFile(const std::string &path, Folder *parent)
		{
			return CreateFile(std::tr2::sys::path(path), parent);
		}
	}
}
