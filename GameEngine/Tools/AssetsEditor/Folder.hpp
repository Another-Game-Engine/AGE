#pragma once

#include <memory>
#include <filesystem>
#include <vector>
#include <string>
#include <functional>

#include "File.hpp"

namespace AGE
{
	namespace AE
	{
//		class File;
		class Folder
		{
		public:
			Folder();
			Folder(const std::string &path, Folder* parent = nullptr);
			Folder(const std::tr2::sys::basic_directory_entry<std::tr2::sys::path> &path, Folder *parent = nullptr);
			void list();
			void printImgUi(File::PrintInfos infos = File::PrintInfos(File::Name | File::Type));
			void filesFilter(std::function<bool(const std::tr2::sys::path& file)> &filter);
			void foldersFilter(std::function<bool(const std::tr2::sys::path& file)> &filter);
			void clearFileFilter();
			void clearFolderFilter();
		private:
			std::tr2::sys::basic_directory_entry<std::tr2::sys::path> _path;
			std::vector < Folder > _folders;
			Folder *_parent;
			bool _active;
			//std::vector < File > _files;
		};
	}
}