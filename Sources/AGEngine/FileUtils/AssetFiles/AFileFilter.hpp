#pragma once

#include <memory>
#include <FileUtils/AssetFiles/File.hpp>

namespace AGE
{
	namespace FileUtils
	{
		class AFileFilter
		{
		public:
			AFileFilter() = default;
			virtual ~AFileFilter() = default;

			virtual bool isValidFile(const std::tr2::sys::path &path) = 0;
			virtual std::shared_ptr<AssetFile> ageCreateFile(const std::tr2::sys::path &path, Folder *parent = nullptr) = 0;
			virtual std::shared_ptr<AssetFile> ageCreateFile(const std::string &path, Folder *parent = nullptr) = 0;
		};
	}
}