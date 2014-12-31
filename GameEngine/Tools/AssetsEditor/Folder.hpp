#pragma once

#include <memory>
#include <filesystem>
#include <vector>
#include <string>

namespace AGE
{
	namespace AE
	{
//		class File;
		class Folder : public std::enable_shared_from_this < Folder >
		{
		public:
			Folder();
			Folder(const std::string &path, Folder* parent = nullptr);
			Folder(const std::tr2::sys::basic_directory_entry<std::tr2::sys::path> &path, Folder *parent = nullptr);
			void list();
			void printImgUi();
		private:
			std::tr2::sys::basic_directory_entry<std::tr2::sys::path> _path;
			std::vector < Folder > _folders;
			Folder *_parent;
			//std::vector < File > _files;
		};
	}
}