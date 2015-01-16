#pragma once

#include <File.hpp>

namespace AGE
{
	namespace AE
	{
		class RawTextureFile : public AssetFile
		{
		public:
			virtual ~RawTextureFile();
			RawTextureFile(const AE::Path &path, Folder *parent = nullptr);
		};
	}
}