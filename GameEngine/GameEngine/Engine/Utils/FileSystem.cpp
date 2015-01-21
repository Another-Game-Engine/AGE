#include "FileSystem.hpp"
#include <filesystem>

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
	}
}