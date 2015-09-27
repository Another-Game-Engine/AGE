#pragma once

#include <FileUtils/AssetFiles/File.hpp>
#include <set>

namespace AGE
{
	struct AssetDataSet;

	namespace FileUtils
	{
		class CookedFile;

		class RawFile : public FileUtils::AssetFile
		{
		public:
			virtual ~RawFile();
			RawFile(const FileUtils::Path &path, FileUtils::Folder *parent = nullptr);

			virtual void cook() = 0;
			virtual void selection() = 0;

			bool _dirty;
			std::shared_ptr<CookedFile> _cookedFile;
			std::shared_ptr<AssetDataSet> dataSet;
		};
	}
}