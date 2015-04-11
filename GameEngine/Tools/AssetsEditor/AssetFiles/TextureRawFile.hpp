#pragma once

#include "RawFile.hpp"

namespace AGE
{
	struct AssetDataSet;
	namespace AE
	{
		class CookedFile;
		struct CookConfig;

		class TextureRawFile : public RawFile
		{
		public:
			virtual ~TextureRawFile();
			TextureRawFile(const AE::Path &path, Folder *parent = nullptr);
			virtual void cook();
			virtual void selection();
		};
	}
}