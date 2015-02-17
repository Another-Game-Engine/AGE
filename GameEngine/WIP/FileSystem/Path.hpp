#pragma once

#include <string>

namespace Age
{
	namespace Engine
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

			static std::string Path::DirectoryName(const char *str);
			static std::string BaseName(const char *str);
			static std::string PathName(const char *str);
			static std::string Path::AbsoluteName(const char *path, const char *str);
			static std::string Path::RelativeName(const char *path, const char *str);
			static std::string Path::Extension(const char *str, const char *ext);
		};
	}
}