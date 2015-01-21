#pragma once

#include "RawFile.hpp"

namespace AGE
{
	namespace AE
	{
		class UnknownFile : public RawFile
		{
		public:
			virtual ~UnknownFile()
			{}
			UnknownFile(const std::tr2::sys::path &path, Folder *parent = nullptr)
				: RawFile(path, "Unknown", parent)
			{
			}
		};
	}
}