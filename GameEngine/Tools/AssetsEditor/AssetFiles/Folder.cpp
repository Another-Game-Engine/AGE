#include "Folder.hpp"

#include <imgui\imgui.h>
#include <regex>

#include "AssetFileManager.hpp"
#include <Utils/FileSystem.hpp>
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

		void Folder::list()
		{
			_folders.clear();
			_files.clear();
			for (auto it = std::tr2::sys::directory_iterator(_path)
				;  it != std::tr2::sys::directory_iterator()
				; ++it)
			{
				const auto& file = it->path();

				if (!std::tr2::sys::is_directory(file))
				{
					auto n = AssetFileManager::CreateFile(file.relative_path(), this);
					if (n)
					{
						_files.push_back(n);
					}
				}
				else if (std::tr2::sys::is_directory(file))
				{
					auto n = std::make_shared<AE::Folder>(std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(_path.path().string() + "/" + file.filename() + "/"), this);
					_folders.push_back(n);
					n->list();
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
				if (!filter(e->_path))
					e->_active = false;
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
				e->_active = true;
				e->clearFolderFilter();
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
					e->_internalFind(path, result);
				}
				if (result == nullptr)
				{
					for (auto &e : _files)
					{
						if (e->getPath() == path)
						{
							result = e;
							return;
						}
					}
				}
			}
		}

		void Folder::find(const std::string &path, std::shared_ptr<AssetFile> &result)
		{
			_internalFind(FileSystem::CleanPath(path), result);
		}
	}
}