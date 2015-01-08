#pragma once

#include <File.hpp>

namespace AGE
{
	namespace AE
	{
		class RawMaterialFile : public AssetFile
		{
		public:
			virtual ~RawMaterialFile();
			RawMaterialFile(const AE::Path &path, Folder *parent = nullptr);
		};
	}
}