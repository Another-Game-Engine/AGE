#pragma once

#include <string>

namespace AGE
{
	namespace FileSystem
	{
		bool CreateFolder(const std::string &path);
		bool Exists(const std::string &path);
	}
}