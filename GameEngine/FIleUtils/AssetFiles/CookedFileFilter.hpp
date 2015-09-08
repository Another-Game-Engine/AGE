#pragma once

#include "AFileFilter.hpp"

namespace AGE
{
	namespace FileUtils
	{
		class CookedFileFilter : public AFileFilter
		{
		public:
			CookedFileFilter() = default;
			~CookedFileFilter() = default;

			virtual bool isValidFile(const std::tr2::sys::path &path);
			virtual std::shared_ptr<AssetFile> ageCreateFile(const std::tr2::sys::path &path, Folder *parent = nullptr);
			virtual std::shared_ptr<AssetFile> ageCreateFile(const std::string &path, Folder *parent = nullptr);
		};
	}
}

