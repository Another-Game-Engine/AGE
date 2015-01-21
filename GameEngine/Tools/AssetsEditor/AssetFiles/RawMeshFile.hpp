#pragma once

#include "RawFile.hpp"

namespace AGE
{
	namespace AE
	{
		class RawMeshFile : public RawFile
		{
		public:
			virtual ~RawMeshFile();
			RawMeshFile(const AE::Path &path, Folder *parent = nullptr);
		};
	}
}