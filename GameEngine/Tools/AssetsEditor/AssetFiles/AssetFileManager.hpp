#pragma once

#include <FileUtils/AssetFiles/AssetsTypes.hpp>
#include <set>
#include <filesystem>

namespace AGE
{
	namespace FileUtils
	{
		class AssetFile;
		class RawFile;
		class Folder;
	}
	namespace AE
	{
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
			static void PrintSelectableRawAssetsFile(FileUtils::RawFile *ptr, int printSections, std::set<std::shared_ptr<FileUtils::RawFile>> *list);
			static void PrintClickableRawAssetsFile(FileUtils::RawFile *ptr, int printSections, std::shared_ptr<FileUtils::RawFile> &clicked);
			static void CheckIfRawModified(FileUtils::Folder *folder, std::set <std::shared_ptr<FileUtils::RawFile>> &list);
			static void LinkRawToCooked(FileUtils::Folder *raw, FileUtils::Folder *cooked);
			static std::string RawPathToCooked(const std::string &path);
		private:
			AssetFileManager() = delete;
		};
	}
}