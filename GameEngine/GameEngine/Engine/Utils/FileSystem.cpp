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
			return std::tr2::sys::create_directories(path);
		}

		bool Exists(const std::string &path)
		{
			std::tr2::sys::exists(path);
		}
	}
}