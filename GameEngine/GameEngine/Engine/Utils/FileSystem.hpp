#pragma once

#include <string>

namespace AGE
{
	namespace FileSystem
	{
		bool CreateFolder(const std::string &path);
		bool Exists(const std::string &path);
		std::string GetExtension(const std::string &path);
		std::string GetDateStr(const std::string &path, const char* format = "%Y/%m/%d %H:%M:%S");
		struct tm GetLastWriteTime(const std::string &path);
	}
}