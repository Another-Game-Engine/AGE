#include "RawTextureFile.hpp"

namespace AGE
{
	namespace AE
	{
			RawTextureFile::~RawTextureFile()
			{
			}
			RawTextureFile::RawTextureFile(const AE::Path &path, Folder *parent)
				: RawFile(path, "Texture | Raw", parent)
			{
				_color = { { 0.02f, 0.4f, 0.8f } };
			}
	}
}