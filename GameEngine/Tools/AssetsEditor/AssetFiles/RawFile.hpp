#pragma once

#include <File.hpp>

namespace AGE
{
	namespace AE
	{
		class RawFile : public AssetFile
		{
		public:
			virtual ~RawFile();
			RawFile(const AE::Path &path, const std::string &type, Folder *parent = nullptr);
		};
	}
}