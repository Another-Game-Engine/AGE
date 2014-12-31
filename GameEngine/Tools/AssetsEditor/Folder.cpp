#include "Folder.hpp"

//tmp
#include <iostream>
#include <imgui\imgui.h>

namespace AGE
{
	namespace AE
	{
		Folder::Folder()
		{}

		Folder::Folder(const std::string &path, Folder *parent)
			: _parent(parent)
		{
			auto strPath = path;
			if (strPath.back() == '/')
				strPath.pop_back();
			_path = std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(path);
		}

		Folder::Folder(const std::tr2::sys::basic_directory_entry<std::tr2::sys::path> &path, Folder *parent)
			: _path(path)
			, _parent(parent)
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

				//auto matches_mask = [](const path& file) {
				//	return regex_search(file.filename(), regex(".*curletts.*\\.km(l|z)"));
				//};

				if (!std::tr2::sys::is_directory(file)/* && matches_mask(file)*/)
				{
					//auto last_wrinte = last_write_time(file);
					std::cout << "file : " << file.relative_path().filename() << " - "
						<< /*put_time(localtime(&last_write), "%Y/%m/%d %H:%M:%S") << */ std::endl;
				}
				else if (std::tr2::sys::is_directory(file)/* && matches_mask(file)*/)
				{
					_folders.emplace_back(std::tr2::sys::basic_directory_entry<std::tr2::sys::path>(_path.path().string() + "/" + file.filename() + "/"), this);
					_folders.back().list();
					//auto last_wrinte = last_write_time(file);
					std::cout << "folder : " << file.filename() << " - "
						<< /*put_time(localtime(&last_write), "%Y/%m/%d %H:%M:%S") << */ std::endl;
				}
			}
		}

		void Folder::printImgUi()
		{
			if (ImGui::TreeNode(_path.path().filename().c_str()))
			{
				for (auto &e : _folders)
				{
					e.printImgUi();
				}
				ImGui::TreePop();
			}
		}
	}
}