#include "RawMeshFile.hpp"

namespace AGE
{
	namespace AE
	{
			RawMeshFile::~RawMeshFile()
			{
			}
			RawMeshFile::RawMeshFile(const std::tr2::sys::path &path, Folder *parent)
				: RawFile(path, "Mesh | Raw", parent)
			{
				_color = { { 0.8f, 0.05f, 0.5f } };
			}
	}
}