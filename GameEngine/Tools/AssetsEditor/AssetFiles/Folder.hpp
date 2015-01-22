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
			void filesFilter(std::function<bool(const std::tr2::sys::path& file)> &filter);
			void foldersFilter(std::function<bool(const std::tr2::sys::path& file)> &filter);
			void clearFileFilter();
			void clearFolderFilter();

			template <typename T>
			void update(std::function<void(Folder*)> &folderFunction, std::function<void(T*)> &fileFunction)
			{
				folderFunction(this);
				for (auto &e : _folders)
				{
					e->update(folderFunction, fileFunction);
				}
				for (auto &e : _files)
				{
					e->update(fileFunction);
				}
			}

			template <typename T>
			void update(std::function<void(T*)> &fileFunction)
			{
				folderFunction(this);
				for (auto &e : _folders)
				{
					e->update(fileFunction);
				}
				for (auto &e : _files)
				{
					e->update(fileFunction);
				}
			}

			template <typename T>
			void update(std::function<bool(Folder*)> &folderFunctionBegin
				, std::function<bool(Folder*)> &folderFunctionEnd
				, std::function<void(T*)> &fileFunction)
			{
				if (!folderFunctionBegin(this))
					return;
				for (auto &e : _folders)
				{
					e->update(folderFunctionBegin, folderFunctionEnd, fileFunction);
				}
				for (auto &e : _files)
				{
					e->update(fileFunction);
				}
				folderFunctionEnd(this);
			}

			std::tr2::sys::basic_directory_entry<std::tr2::sys::path> _path;
			std::vector < std::shared_ptr<AE::Folder> > _folders;
			std::vector < std::shared_ptr<AE::AssetFile> > _files;
			Folder *_parent;
			bool _active;
		};
	}
}