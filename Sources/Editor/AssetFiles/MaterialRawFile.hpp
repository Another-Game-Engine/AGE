#pragma once

#include <FileUtils/AssetFiles/RawFile.hpp>

namespace AGE
{
	struct AssetDataSet;
	namespace AE
	{
		class CookedFile;
		struct CookConfig;

		class MaterialRawFile : public FileUtils::RawFile
		{
		public:
			virtual ~MaterialRawFile();
			MaterialRawFile(const FileUtils::Path &path, FileUtils::Folder *parent = nullptr);
			virtual void cook();
			virtual void selection();
		};
	}
}