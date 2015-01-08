#pragma once

#include <File.hpp>

namespace AGE
{
	namespace AE
	{
		class RawMeshFile : public AssetFile
		{
		public:
			virtual ~RawMeshFile();
			RawMeshFile(const AE::Path &path, Folder *parent = nullptr);
		};
	}
}