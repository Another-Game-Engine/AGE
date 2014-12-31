#pragma once

#include <memory>
#include <filesystem>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include "File.hpp"

namespace AGE
{
	namespace AE
	{
		class Folder
		{
		public:
			Folder();
			Folder(const std::string &path, Folder* parent = nullptr);
			Folder(const std::tr2::sys::basic_directory_entry<std::tr2::sys::path> &path, Folder *parent = nullptr);
			void list();
			void printImgUi(AssetFile::PrintInfos infos = AssetFile::Name | AssetFile::Type);
			void filesFilter(std::function<bool(const std::tr2::sys::path& file)> &filter);
			void foldersFilter(std::function<bool(const std::tr2::sys::path& file)> &filter);
			void clearFileFilter();
			void clearFolderFilter();
		private:
			std::tr2::sys::basic_directory_entry<std::tr2::sys::path> _path;
			std::vector < std::shared_ptr<AE::Folder> > _folders;
			std::vector < std::shared_ptr<AE::AssetFile> > _files;
			Folder *_parent;
			bool _active;
		};
	}
}