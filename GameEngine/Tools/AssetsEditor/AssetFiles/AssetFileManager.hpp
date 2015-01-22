#pragma once

#include "AssetFiles/File.hpp"
#include <set>

namespace AGE
{
	namespace AE
	{
		class RawFile;

		class AssetFileManager
		{
		public:
			enum PrintSection
			{
				Name = 1 << 0
				, Path = 1 << 1
				, Type = 1 << 2
				, Date = 1 << 3
			};
			static std::shared_ptr<AssetFile> CreateFile(const std::tr2::sys::path &path, Folder *parent = nullptr);
			static std::shared_ptr<AssetFile> CreateFile(const std::string &path, Folder *parent = nullptr);
			static void PrintSelectableRawAssetsFile(RawFile *ptr, int printSections, std::set<std::shared_ptr<RawFile>> *list);
			static void CheckIfRawModified(Folder *folder, std::set <std::shared_ptr<RawFile>> &list);
			static void LinkRawToCooked(Folder *folder);
		private:
			AssetFileManager() = delete;
		};
	}
}