#include "CookedFile.hpp"

namespace AGE
{
	namespace FileUtils
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