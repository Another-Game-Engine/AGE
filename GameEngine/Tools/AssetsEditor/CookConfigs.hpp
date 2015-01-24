#pragma once

#include <string>
#include <memory>
#include <glm/fwd.hpp>

#define MAX_ASSET_NAME_LENGTH 255

namespace AGE
{
	namespace AE
	{
		class CookedFile;
		class RawFile;

		struct CookConfig
		{
			enum CookConfigType
			{
				Animation = 0
				, Material = 1
				, Skin = 2
				, Physic = 3
				, Skeleton = 4
				, Texture = 5
				, UNKNOWN = 6
			};
			static const char * _cookConfigTypeString[];
			inline const char *cookConfigTypeToString() { return _cookConfigTypeString[type]; }

			const CookConfigType type = UNKNOWN;
			char name[MAX_ASSET_NAME_LENGTH];
			std::weak_ptr<CookedFile> cookedFile;
			std::weak_ptr<RawFile> rawFile;
			std::string logs; // used by convertors to log errors and warnings
			struct tm lastTimeEdited;

			CookConfig(CookConfigType t);

			void update();
			void updateLastTimeEdited();
			virtual void drawImGuiTypeSpecific() = 0;
		protected:
			int modified;
		};

		struct AnimationConfig : public CookConfig
		{
			AnimationConfig();
			virtual void drawImGuiTypeSpecific();
		};

		struct MaterialConfig : public CookConfig
		{
			MaterialConfig();
			virtual void drawImGuiTypeSpecific();
		};

		struct SkinConfig : public CookConfig
		{
			SkinConfig();
			virtual void drawImGuiTypeSpecific();
			bool normalize = true;
			float maxSideLength = 1.0f;
			bool positions = true;
			bool normals = true;
			bool bonesInfos = false;
			bool uvs = true;
			bool tangents = true;
			bool biTangents = true;
		};

		struct PhysicConfig : public CookConfig
		{
			PhysicConfig();
			virtual void drawImGuiTypeSpecific();
			enum Type
			{
				staticConcave = 0
				, dynamicConcave = 1
				, sphere = 2
				, cube = 3
			};
			Type rigidBodyType;
			static const char * _typeString[];
			static inline const char *TypeToString(int t) { return _typeString[t]; }
		};

		struct SkeletonConfig : public CookConfig
		{
			SkeletonConfig();
			virtual void drawImGuiTypeSpecific();
		};

		struct TextureConfig : public CookConfig
		{
			TextureConfig();
			virtual void drawImGuiTypeSpecific();
		};
	}
}