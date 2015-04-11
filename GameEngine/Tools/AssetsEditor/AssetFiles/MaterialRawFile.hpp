#pragma once

#include "RawFile.hpp"

namespace AGE
{
	struct AssetDataSet;
	namespace AE
	{
		class CookedFile;
		struct CookConfig;

		class MaterialRawFile : public RawFile
		{
		public:
			virtual ~MaterialRawFile();
			MaterialRawFile(const AE::Path &path, Folder *parent = nullptr);
			virtual void cook();
			virtual void selection();
		};
	}
}