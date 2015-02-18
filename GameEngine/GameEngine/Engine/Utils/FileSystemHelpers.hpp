#pragma once

#include <string>

namespace AGE
{
	namespace FileSystemHelpers
	{
		bool CreateFolder(const std::string &path);
		bool Exists(const std::string &path);
		std::string GetExtension(const std::string &path);
		std::string GetDateStr(const std::string &path, const char* format = "%Y/%m/%d %H:%M:%S");
		struct tm GetLastWriteTime(const std::string &path);
		double GetDiffTime(struct tm &first, struct tm &second);
		double GetDiffTime(const std::string &first, const std::string &second);
		std::string CleanPath(const std::string &path);
	}
}