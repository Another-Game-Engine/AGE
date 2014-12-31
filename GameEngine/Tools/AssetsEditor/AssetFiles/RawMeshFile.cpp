#include "RawMeshFile.hpp"

namespace AGE
{
	namespace AE
	{
			RawMeshFile::~RawMeshFile()
			{
			}
			RawMeshFile::RawMeshFile(const std::tr2::sys::path &path, Folder *parent)
				: AssetFile(path, "Mesh | Raw", parent)
			{}
	}
}