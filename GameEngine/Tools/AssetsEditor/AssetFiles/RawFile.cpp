#include "RawFile.hpp"

namespace AGE
{
	namespace AE
	{
		RawFile::~RawFile()
		{
		}
		RawFile::RawFile(const std::tr2::sys::path &path, const std::string &type, Folder *parent)
			: AssetFile(path, type, parent)
		{
		}
	}
}