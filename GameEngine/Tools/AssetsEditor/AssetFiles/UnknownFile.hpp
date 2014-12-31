#pragma once

#include <File.hpp>

namespace AGE
{
	namespace AE
	{
		class UnknownFile : public AssetFile
		{
		public:
			virtual ~UnknownFile()
			{}
			UnknownFile(const std::tr2::sys::path &path, Folder *parent = nullptr)
				: AssetFile(path, "Unknown", parent)
			{}
		};
	}
}