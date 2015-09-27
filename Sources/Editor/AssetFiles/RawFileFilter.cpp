#include "RawFileFilter.hpp"

#include "AssetFileManager.hpp"

#include <FileUtils/AssetFiles/File.hpp>
#include <FileUtils/FileSystemHelpers.hpp>
#include <FileUtils/AssetFiles/AssetsTypes.hpp>

#include "AssetFiles/MeshRawFile.hpp"
#include "AssetFiles/MaterialRawFile.hpp"
#include "AssetFiles/TextureRawFile.hpp"

namespace AGE
{
	bool RawFileFilter::isValidFile(const std::tr2::sys::path &path)
	{
		auto extension = FileUtils::GetExtension(path);
		if (extension == "obj" || extension == "fbx" || extension == "dae")
			return true;
		if (extension == "bmp" || extension == "jpg" || extension == "jpeg" || extension == "tga" || extension == "png" || extension == "tif")
			return true;
		if (extension == "mtl")
			return true;
		return false;
	}

	std::shared_ptr<FileUtils::AssetFile> RawFileFilter::ageCreateFile(const std::tr2::sys::path &path, FileUtils::Folder *parent)
	{
		auto extension = FileUtils::GetExtension(path);
		std::shared_ptr<FileUtils::AssetFile> t = nullptr;
		if (extension == "obj" || extension == "fbx" || extension == "dae")
		{
			t = std::make_shared<AE::MeshRawFile>(path, parent);
			t->_type = FileUtils::AssetType::Raw | FileUtils::AssetType::Mesh;
		}
		else if (extension == "bmp" || extension == "jpg" || extension == "jpeg" || extension == "tga" || extension == "png" || extension == "tif")
		{
			t = std::make_shared<AE::TextureRawFile>(path, parent);
			t->_type = FileUtils::AssetType::Raw | FileUtils::AssetType::Texture;
		}
		else if (extension == "mtl")
		{
			t = std::make_shared<AE::MaterialRawFile>(path, parent);
			t->_type = FileUtils::AssetType::Raw | FileUtils::AssetType::Material;
		}

		if (t)
		{
			t->_lastWriteTime = FileUtils::GetLastWriteTime(t->getPath());
			t->_lastWriteTimeStr = FileUtils::GetDateStr(t->getPath());
		}
		return t;
	}
	
	std::shared_ptr<FileUtils::AssetFile> RawFileFilter::ageCreateFile(const std::string &path, FileUtils::Folder *parent)
	{
		return ageCreateFile(std::tr2::sys::path(path), parent);
	}
}