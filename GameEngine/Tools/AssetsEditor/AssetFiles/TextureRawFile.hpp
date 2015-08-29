#pragma once

#include <FileUtils/AssetFiles/RawFile.hpp>

namespace AGE
{
	struct AssetDataSet;
	namespace AE
	{
		class CookedFile;
		struct CookConfig;

		class TextureRawFile : public FileUtils::RawFile
		{
		public:
			virtual ~TextureRawFile();
			TextureRawFile(const FileUtils::Path &path, FileUtils::Folder *parent = nullptr);
			virtual void cook();
			virtual void selection();
		};
	}
}