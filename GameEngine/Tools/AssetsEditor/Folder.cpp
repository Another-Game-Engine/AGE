#include "Folder.hpp"

#include <imgui\imgui.h>
#include <regex>

//tmp
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
			_path = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(path);
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
			for (auto it = std::tr2::sys::directory_iterator(_path);
				it != std::tr2::sys::directory_iterator(); ++it)
			{
				// file object contains absolute path in the case of recursive iterators
				const auto& file = it->path();

				if (!std::tr2::sys::is_directory(file)/* && matches_mask(file)*/)
				{
					//auto last_wrinte = last_write_time(file);
					std::cout << "file : " << file.relative_path().filename() << " - "
						<< /*put_time(localtime(&last_write), "%Y/%m/%d %H:%M:%S") << */ std::endl;
				}
				else if (std::tr2::sys::is_directory(file))
				{
					_folders.emplace_back(std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(_path.path().string() + "/" + file.filename() + "/"), this);
					_folders.back().list();
					//auto last_wrinte = last_write_time(file);
					std::cout << "folder : " << file.filename() << " - "
						<< /*put_time(localtime(&last_write), "%Y/%m/%d %H:%M:%S") << */ std::endl;
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
				if (!filter(e._path))
					e._active = false;
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
				e._active = true;
				e.clearFolderFilter();
			}
		}


		void Folder::printImgUi(File::PrintInfos infos)
		{
			if (ImGui::TreeNode((void*)(this), _path.path().filename().c_str()))
			{
				for (auto &e : _folders)
				{
					if (e._active)
						e.printImgUi();
				}
				ImGui::TreePop();
			}
		}
	}
}