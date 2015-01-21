#pragma once

#include "AssetFiles/File.hpp"

namespace AGE
{
	namespace AE
	{
		class AssetFileManager
		{
		public:
			static std::shared_ptr<AssetFile> CreateFile(const std::tr2::sys::path &path, Folder *parent = nullptr);
			static std::shared_ptr<AssetFile> CreateFile(const std::string &path, Folder *parent = nullptr);
		private:
			AssetFileManager() = delete;
		};
	}
}