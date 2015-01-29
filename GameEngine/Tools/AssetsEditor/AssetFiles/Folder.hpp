#pragma once

#include <memory>
#include <filesystem>
#include <map>
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
			void find(const std::string &path, std::shared_ptr<AssetFile> &result);

		private:
			void _internalFind(const std::string &path, std::shared_ptr<AssetFile> &result);
		public:

			template <typename T>
			void update(std::function<void(Folder*)> &folderFunction, std::function<void(T*)> &fileFunction)
			{
				folderFunction(this);
				for (auto &e : _folders)
				{
					e.second->update(folderFunction, fileFunction);
				}
				for (auto &e : _files)
				{
					e.second->update(fileFunction);
				}
			}

			template <typename T>
			void update(std::function<void(T*)> &fileFunction)
			{
				for (auto &e : _folders)
				{
					e.second->update(fileFunction);
				}
				for (auto &e : _files)
				{
					e.second->update(fileFunction);
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
					e.second->update(folderFunctionBegin, folderFunctionEnd, fileFunction);
				}
				for (auto &e : _files)
				{
					e.second->update(fileFunction);
				}
				folderFunctionEnd(this);
			}

			std::tr2::sys::basic_directory_entry<std::tr2::sys::path> _path;
			std::map<std::string, std::shared_ptr<AE::Folder> > _folders;
			std::map<std::string, std::shared_ptr<AE::AssetFile> > _files;
			Folder *_parent;
			bool _active;
		};
	}
}