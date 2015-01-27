#pragma once

#include "File.hpp"
#include <set>

namespace AGE
{
	struct AssetDataSet;
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
			std::shared_ptr<AssetDataSet> dataSet;
		};
	}
}