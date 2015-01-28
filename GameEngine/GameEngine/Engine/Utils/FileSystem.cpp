#include "FileSystem.hpp"
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace AGE
{
	namespace FileSystem
	{
		bool CreateFolder(const std::string &path)
		{
			if (Exists(path))
				return true;
			return std::tr2::sys::create_directories(std::tr2::sys::path(path));
		}

		bool Exists(const std::string &path)
		{
			return std::tr2::sys::exists(std::tr2::sys::path(path));
		}

		std::string GetExtension(const std::string &path)
		{
			std::string::size_type		pos;
			pos = path.find_last_of(".");
			if (pos != std::string::npos)
				return path.substr(pos + 1, std::string::npos);
			else
				return "";
		}

		std::string GetDateStr(const std::string &path, const char *format /*= "%Y/%m/%d %H:%M:%S"*/)
		{
			std::stringstream res;
			auto timeinfo = GetLastWriteTime(path);
			res << std::put_time(&timeinfo, format);
			return res.str();
		}

		struct tm GetLastWriteTime(const std::string &path)
		{
			auto last_write = std::tr2::sys::last_write_time(std::tr2::sys::path(path));
			struct tm timeinfo;
			localtime_s(&timeinfo, &last_write);
			return timeinfo;
		}

		double GetDiffTime(struct tm &first, struct tm &second)
		{
			return difftime(mktime(&first), mktime(&second));
		}

		double GetDiffTime(const std::string &first, const std::string &second)
		{
			return GetDiffTime(GetLastWriteTime(first), GetLastWriteTime(second));
		}

		std::string CleanPath(const std::string &path)
		{
			auto r = path;
			std::replace(r.begin(), r.end(), '\\', '/');
			return r;
		}
	}
}