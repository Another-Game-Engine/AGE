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

			virtual void cook() = 0;
			virtual void selection() = 0;

			bool _dirty;
			std::shared_ptr<CookedFile> _cookedFile;
			std::shared_ptr<AssetDataSet> dataSet;
		};
	}
}