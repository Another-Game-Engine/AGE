#pragma once

namespace AGE
{
	namespace FileUtils
	{
		enum AssetType
		{
			Raw = 1 << 0
			, Cooked = 1 << 1
			, Mesh = 1 << 2
			, Material = 1 << 3
			, Texture = 1 << 4
			, Animation = 1 << 5
			, Skeleton = 1 << 6
		};
	}
}