#include "CookedFile.hpp"
#include "RawFile.hpp"

namespace AGE
{
	namespace AE
	{
		CookedFile::~CookedFile()
		{
		}
		CookedFile::CookedFile(const std::tr2::sys::path &path, Folder *parent)
			: AssetFile(path, parent)
		{
		}
	}
}