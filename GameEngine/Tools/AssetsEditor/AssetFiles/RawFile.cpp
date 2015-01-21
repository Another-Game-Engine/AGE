#include "RawFile.hpp"

namespace AGE
{
	namespace AE
	{
		RawFile::~RawFile()
		{
		}
		RawFile::RawFile(const std::tr2::sys::path &path, Folder *parent)
			: AssetFile(path, parent)
			, _dirty(true)
		{
		}
	}
}