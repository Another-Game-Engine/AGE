#pragma once

#include <string>

namespace AGE
{
	class Path final
	{
	public:
		Path(void) = delete;
		Path(const Path &other) = delete;
		Path(Path &&other) = delete;
		Path &operator=(const Path &other) = delete;
		Path &operator=(Path &&other) = delete;
		~Path(void) = delete;

		static std::string DirectoryName(const char *str);
		static std::string BaseName(const char *str);
		static std::string PathName(const char *str);
		static std::string AbsoluteName(const char *path, const char *str);
		static std::string RelativeName(const char *path, const char *str);
		static std::string Extension(const char *str, const char *ext);
	};
}