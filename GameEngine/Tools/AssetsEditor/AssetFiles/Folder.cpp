#include "Folder.hpp"

#include <imgui\imgui.h>
#include <regex>

#include "AssetFileManager.hpp"
#include <Utils/FileSystemHelpers.hpp>
#include <iostream>

namespace AGE
{
	namespace AE
	{
		Folder::Folder()
			: _active(true)
		{}

		Folder::Folder(const std::string &path, Folder *parent)
			: _parent(parent)
			, _active(true)
		{
			auto strPath = path;
			if (strPath.back() == '/')
				strPath.pop_back();
			_path = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(strPath);
		}

		Folder::Folder(const std::tr2::sys::basic_directory_entry<std::tr2::sys::path> &path, Folder *parent)
			: _path(path)
			, _parent(parent)
			, _active(true)
		{
			if (path.path().string().back() == '/')
			{
				auto s = path.path().string();
				s.pop_back();
				_path = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(s);
			}
		}

		void Folder::list(const std::string &path)
		{
			if (path.size() != 0)
			{
				auto strPath = path;
				if (strPath.back() == '/')
					strPath.pop_back();
				_path = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(strPath);
			}

			int dirCount = 0;
			int fileCount = 0;
			for (auto it = std::tr2::sys::directory_iterator(_path)
				;  it != std::tr2::sys::directory_iterator()
				; ++it)
			{
				const auto& file = it->path();

				if (!std::tr2::sys::is_directory(file) /*&& AssetFileManager::IsValidFile(file)*/)
				{
					fileCount++;
					if (_files.find(file.relative_path().string()) == std::end(_files))
					{
						auto n = AssetFileManager::AgeCreateFile(file.relative_path(), this);
						if (n)
						{
							_files.insert(std::make_pair(file.relative_path().string(), n));
						}
					}
				}
				else if (std::tr2::sys::is_directory(file))
				{
					dirCount++;
					if (_folders.find(file.filename()) == std::end(_folders))
					{
						auto n = std::make_shared<AE::Folder>(std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(_path.path().string() + "/" + file.filename() + "/"), this);
						_folders.insert(std::make_pair(file.filename(), n));
						n->list();
					}
				}
			}
			if (dirCount != _folders.size())
			{
				for (auto it = std::begin(_folders); it != std::end(_folders); )
				{
					if (!std::tr2::sys::exists(it->second->_path.path()))
					{
						_folders.erase(it++);
					}
					else
						++it;
				}
			}
			if (fileCount != _files.size())
			{
				for (auto it = std::begin(_files); it != std::end(_files);)
				{
					if (!std::tr2::sys::exists(it->second->getFsPath()))
					{
						_files.erase(it++);
					}
					else
						++it;
				}
			}
		}

		void Folder::filesFilter(std::function<bool(const std::tr2::sys::path& file)> &filter)
		{
			//todo
			clearFileFilter();
		}

		void Folder::foldersFilter(std::function<bool(const std::tr2::sys::path& file)> &filter)
		{
			clearFolderFilter();
			for (auto &e : _folders)
			{
				if (!filter(e.second->_path))
					e.second->_active = false;
			}
		}

		void Folder::clearFileFilter()
		{
			//todo
		}

		void Folder::clearFolderFilter()
		{
			_active = true;
			for (auto &e : _folders)
			{
				e.second->_active = true;
				e.second->clearFolderFilter();
			}
		}

		void Folder::_internalFind(const std::string &path, std::shared_ptr<AssetFile> &result)
		{
			if (result != nullptr)
				return;
			std::string::size_type f = path.find(_path.path().string());
			if (f != std::string::npos && f == 0)
			{
				for (auto &e : _folders)
				{
					e.second->_internalFind(path, result);
				}
				if (result == nullptr)
				{
					for (auto &e : _files)
					{
						if (e.second->getPath() == path)
						{
							result = e.second;
							return;
						}
					}
				}
			}
		}

		void Folder::find(const std::string &path, std::shared_ptr<AssetFile> &result)
		{
			_internalFind(FileSystemHelpers::CleanPath(path), result);
		}
	}
}