#pragma once

#include <string>
#include <memory>

namepsace AGE
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
			struct tm lastTimeEdited;
		};

		struct AnimationConfig : public CookConfig
		{};

		struct MaterialConfig : public CookConfig
		{};

		struct SkinConfig : public CookConfig
		{};

		struct BulletConfig : public CookConfig
		{};

		struct SkeletonConfig : public CookConfig
		{};

		struct TextureConfig : public CookConfig
		{};
	}
}