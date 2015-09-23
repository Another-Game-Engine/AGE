#ifdef VISUAL_LEAK_DETECTOR
#include <vld.h>
#endif

#include "File.hpp"
#include "Folder.hpp"
#include <iomanip>
#include <sstream>
#include <imgui/imgui.h>
#include "LowLevelUtils/BitOperations.hpp"

namespace AGE
{
	namespace FileUtils
	{
		AssetFile::AssetFile(const std::tr2::sys::path &path, Folder *parent)
			: _active(true)
			, _path(path)
			, _folder(parent)
			, _type(-1)
			, _selected(false)
		{
			_pathStr = _path.string();
			_fileName = _path.filename();
		}

		AssetFile::~AssetFile()
		{
		}
	}
}