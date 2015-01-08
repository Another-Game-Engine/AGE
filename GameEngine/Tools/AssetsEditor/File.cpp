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
			AssetFile::AssetFile(const std::tr2::sys::path &path, const std::string &type, Folder *parent)
				: _active(true)
				, _path(path)
				, _folder(parent)
				, _type(type)
			{
				_color.fill(0.5f);
			}

			AssetFile::~AssetFile()
			{
			}

			std::string AssetFile::getDate()
			{
				auto last_write = std::tr2::sys::last_write_time(_path);
				std::stringstream res;
				struct tm timeinfo;
				localtime_s(&timeinfo, &last_write);
				res << std::put_time(&timeinfo, "%Y/%m/%d %H:%M:%S");
				return res.str();
			}

			void AssetFile::printImgUi(AssetFile::PrintInfos infos)
			{
				if (!_active)
					return;
				if (infos == 0)
				{
					infos = AssetFile::PrintInfosTypes::END;
				}
				auto t = countOne(infos);
				ImGui::Columns(t);

				if (infos & Name)
				{
					ImGui::Text(_path.filename().c_str());
					ImGui::NextColumn();
				}
				if (infos & Type)
				{
					ImGui::TextColored(ImVec4(_color[0], _color[1], _color[2], 1.0f), _type.c_str());
					ImGui::NextColumn();
				}
				if (infos & Date)
				{
					ImGui::Text(getDate().c_str());
					ImGui::NextColumn();
				}
				if (infos & Path)
				{
					ImGui::Text(_path.string().c_str());
					ImGui::NextColumn();
				}
				ImGui::Columns(1);
			}

			std::string AssetFile::getExtension(const std::string &path)
			{
				std::string::size_type		pos;
				pos = path.find_last_of(".");
				if (pos != std::string::npos)
					return path.substr(pos + 1, std::string::npos);
				else
					return "";
			}

			std::string AssetFile::getExtension(const AE::Path &path)
			{
				return getExtension(path.string());
			}
	}
}