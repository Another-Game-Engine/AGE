#include "File.hpp"
#include "Folder.hpp"
#include <iomanip>
#include <sstream>
#include <imgui/imgui.h>
#include <Utils/BitOperations.hpp>

namespace AGE
{
	namespace AE
	{
			AssetFile::AssetFile(const std::tr2::sys::path &path, Folder *parent)
				: _active(true)
				, _path(path)
				, _folder(parent)
				, _type(-1)
				, _selected(false)
			{
				_pathStr = _path.string();
				_fileName = _path.filename();
			}

			AssetFile::~AssetFile()
			{
			}

			//std::string AssetFile::getDate()
			//{
			//	auto last_write = std::tr2::sys::last_write_time(_path);
			//	std::stringstream res;
			//	struct tm timeinfo;
			//	localtime_s(&timeinfo, &last_write);
			//	res << std::put_time(&timeinfo, "%Y/%m/%d %H:%M:%S");
			//	return res.str();
			//}

			//void AssetFile::printImgUi(AssetFile::PrintInfos infos, std::set<std::shared_ptr<AssetFile>> *list)
			//{
			//	if (!_active)
			//		return;
			//	if (infos == 0)
			//	{
			//		infos = AssetFile::PrintInfosTypes::END;
			//	}
			//	auto t = countOne(infos);
			//	ImGui::Columns(t);
			//	if (list && infos & Name)
			//	{
			//		if (ImGui::Checkbox(_path.filename().c_str(), &_selected))
			//		{
			//			if (_selected)
			//			{
			//				list->insert(shared_from_this());
			//			}
			//			else
			//			{
			//				auto f = list->find(shared_from_this());
			//				if (f != list->end())
			//					list->erase(f);
			//			}
			//		}
			//		ImGui::NextColumn();
			//	}
			//	else if (infos & Name)
			//	{
			//		ImGui::Text(_path.filename().c_str());
			//		ImGui::NextColumn();
			//	}
			//	if (infos & Type)
			//	{
			//		ImGui::TextColored(ImVec4(_color[0], _color[1], _color[2], 1.0f), _type.c_str());
			//		ImGui::NextColumn();
			//	}
			//	if (infos & Date)
			//	{
			//		ImGui::Text(getDate().c_str());
			//		ImGui::NextColumn();
			//	}
			//	if (infos & Path)
			//	{
			//		ImGui::Text(_path.string().c_str());
			//		ImGui::NextColumn();
			//	}
			//	ImGui::Columns(1);
			//}
	}
}