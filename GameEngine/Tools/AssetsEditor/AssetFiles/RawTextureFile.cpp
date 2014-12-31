#include "RawTextureFile.hpp"

namespace AGE
{
	namespace AE
	{
			RawTextureFile::~RawTextureFile()
			{
			}
			RawTextureFile::RawTextureFile(const AE::Path &path, Folder *parent)
				: AssetFile(path, "Texture | Raw", parent)
			{}
	}
}