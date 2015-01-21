#pragma once

#include "RawFile.hpp"

namespace AGE
{
	namespace AE
	{
		class RawMaterialFile : public RawFile
		{
		public:
			virtual ~RawMaterialFile();
			RawMaterialFile(const AE::Path &path, Folder *parent = nullptr);
		};
	}
}