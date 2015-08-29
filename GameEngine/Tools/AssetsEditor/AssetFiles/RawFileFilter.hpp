#pragma once

#include <FileUtils/AssetFiles/AFileFilter.hpp>

namespace AGE
{
	class RawFileFilter : public FileUtils::AFileFilter
	{
	public:
		RawFileFilter() = default;
		~RawFileFilter() = default;

		virtual bool isValidFile(const std::tr2::sys::path &path);
		virtual std::shared_ptr<FileUtils::AssetFile> ageCreateFile(const std::tr2::sys::path &path, FileUtils::Folder *parent = nullptr);
		virtual std::shared_ptr<FileUtils::AssetFile> ageCreateFile(const std::string &path, FileUtils::Folder *parent = nullptr);
	};
}

