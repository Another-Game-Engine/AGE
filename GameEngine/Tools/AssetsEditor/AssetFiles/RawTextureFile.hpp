#pragma once

#include "RawFile.hpp"

namespace AGE
{
	namespace AE
	{
		class RawTextureFile : public RawFile
		{
		public:
			virtual ~RawTextureFile();
			RawTextureFile(const AE::Path &path, Folder *parent = nullptr);
		};
	}
}