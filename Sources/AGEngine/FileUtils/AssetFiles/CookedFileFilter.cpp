#include "CookedFileFilter.hpp"
#include "FileUtils/FileSystemHelpers.hpp"
#include "CookedFile.hpp"
#include "AssetsTypes.hpp"

namespace AGE
{
	namespace FileUtils
	{
		bool CookedFileFilter::isValidFile(const std::tr2::sys::path &path)
		{
			auto extension = FileUtils::GetExtension(path);
			if (extension == "dds" || extension == "mage" || extension == "phage" || extension == "sage" || extension == "skage" || extension == "aage")
				return true;
			return false;
		}

		std::shared_ptr<AssetFile> CookedFileFilter::ageCreateFile(const std::tr2::sys::path &path, Folder *parent)
		{
			auto extension = FileUtils::GetExtension(path);
			std::shared_ptr<AssetFile> t = nullptr;

			if (extension == "dds")
			{
				t = std::make_shared<CookedFile>(path, parent);
				t->_type = AssetType::Cooked | AssetType::Texture;
			}
			else if (extension == "mage")
			{
				t = std::make_shared<CookedFile>(path, parent);
				t->_type = AssetType::Cooked | AssetType::Material;
			}
			else if (extension == "phage" || extension == "sage")
			{
				t = std::make_shared<CookedFile>(path, parent);
				t->_type = AssetType::Cooked | AssetType::Mesh;
			}
			else if (extension == "aage")
			{
				t = std::make_shared<CookedFile>(path, parent);
				t->_type = AssetType::Cooked | AssetType::Animation;
			}
			else if (extension == "skage")
			{
				t = std::make_shared<CookedFile>(path, parent);
				t->_type = AssetType::Cooked | AssetType::Skeleton;
			}

			if (t)
			{
				t->_lastWriteTime = FileUtils::GetLastWriteTime(t->getPath());
				t->_lastWriteTimeStr = FileUtils::GetDateStr(t->getPath());
			}
			return t;
		}

		std::shared_ptr<AssetFile> CookedFileFilter::ageCreateFile(const std::string &path, Folder *parent)
		{
			return ageCreateFile(std::tr2::sys::path(path), parent);
		}
	}
}
