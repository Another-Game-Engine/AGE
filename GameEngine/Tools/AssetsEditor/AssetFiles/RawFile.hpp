#pragma once

#include "File.hpp"

namespace AGE
{
	namespace AE
	{
		class RawFile : public AssetFile
		{
		public:
			virtual ~RawFile();
			RawFile(const AE::Path &path, Folder *parent = nullptr);

			bool _dirty;
		};
	}
}