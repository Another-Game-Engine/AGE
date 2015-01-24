#pragma once

#include <string>
#include <memory>
#include <glm/fwd.hpp>

namespace AGE
{
	namespace AE
	{
		class CookedFile;
		class RawFile;

		struct CookConfig
		{
			std::string name;
			std::weak_ptr<CookedFile> cookedFile;
			std::weak_ptr<RawFile> rawFile;
			std::string logs; // used by convertors to log errors and warnings
			struct tm lastTimeEdited;
		};

		struct AnimationConfig : public CookConfig
		{};

		struct MaterialConfig : public CookConfig
		{};

		struct SkinConfig : public CookConfig
		{
			bool normalize;
			float maxSideLength;
			bool positions = true;
			bool normals = true;
			bool bonesInfos = false;
			bool uvs = true;
			bool tangents = true;
			bool biTangents = true;
		};

		struct PhysicConfig : public CookConfig
		{
			enum Type
			{
				staticConcave = 0
				, dynamicConcave = 1
				, sphere = 2
				, cube = 3
			};
		};

		struct SkeletonConfig : public CookConfig
		{};

		struct TextureConfig : public CookConfig
		{};
	}
}