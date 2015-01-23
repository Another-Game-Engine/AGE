#pragma once

#include "File.hpp"

namespace AGE
{
	namespace AE
	{
		class RawFile;
		class CookedFile : public AssetFile
		{
		public:
			virtual ~CookedFile();
			CookedFile(const AE::Path &path, Folder *parent = nullptr);

			std::weak_ptr<RawFile> _rawFile;
		};
	}
}