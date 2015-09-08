#pragma once

#include "File.hpp"

namespace AGE
{
	namespace FileUtils
	{
		class RawFile;
		class CookedFile : public AssetFile
		{
		public:
			virtual ~CookedFile();
			CookedFile(const Path &path, Folder *parent = nullptr);

			std::weak_ptr<RawFile> _rawFile;
		};
	}
}