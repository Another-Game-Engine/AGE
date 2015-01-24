#pragma once

#include "File.hpp"

namespace AGE
{
	namespace AE
	{
		class CookedFile;
		struct CookConfig;

		class RawFile : public AssetFile
		{
		public:
			virtual ~RawFile();
			RawFile(const AE::Path &path, Folder *parent = nullptr);

			bool _dirty;
			std::shared_ptr<CookedFile> _cookedFile;
			std::list < std::shared_ptr<CookConfig> > configs;
		};
	}
}