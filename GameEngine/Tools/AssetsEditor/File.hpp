#pragma once

#include <string>
#include <filesystem>

namespace AGE
{
	namespace AE
	{
		class File
		{
		public:
			enum PrintInfos
			{
				Name = 1 << 0
				, Path = 1 << 1
				, Date = 1 << 2
				, Type = 1 << 3
				, Status = 1 << 4
			};

			File();
			File(const std::string &path);
		private:
		};
	}
}