#pragma once

#include "RawFile.hpp"

namespace AGE
{
	struct AssetDataSet;
	namespace AE
	{
		class CookedFile;
		struct CookConfig;

		class MeshRawFile : public RawFile
		{
		public:
			virtual ~MeshRawFile();
			MeshRawFile(const AE::Path &path, Folder *parent = nullptr);
			virtual void cook();
			virtual void selection();
		};
	}
}