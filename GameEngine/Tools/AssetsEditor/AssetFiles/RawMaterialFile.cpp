#include "RawMaterialFile.hpp"

namespace AGE
{
	namespace AE
	{
			RawMaterialFile::~RawMaterialFile()
			{
			}
			RawMaterialFile::RawMaterialFile(const std::tr2::sys::path &path, Folder *parent)
				: AssetFile(path, "Material | Raw", parent)
			{
				_color = { { 0.3, 0.6, 0.0f } };
			}
	}
}