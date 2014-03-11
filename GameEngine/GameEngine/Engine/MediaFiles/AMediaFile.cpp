#include <MediaFiles/AMediaFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <MediaFiles/ObjFile.hpp>
#include <MediaFiles/TextureFile.hpp>
#include <MediaFiles/CubeMapFile.hpp>
#include <MediaFiles/CollisionShapeStaticFile.hpp>
#include <MediaFiles/CollisionShapeDynamicFile.hpp>
#include <MediaFiles/CollisionBoxFile.hpp>
#include <MediaFiles/CollisionSphereFile.hpp>
#include <MediaFiles/AssetsManager.hpp>
#include <thread>
#include <memory>

void AMediaFile::serializeAsBulletFile(std::ofstream &s)
{
	if (_type == COLLISION_SHAPE_STATIC)
	{
		static_cast<CollisionShapeStaticFile*>(this)->serialize(s);
	}
	else if (_type == COLLISION_SHAPE_DYNAMIC)
	{
		static_cast<CollisionShapeDynamicFile*>(this)->serialize(s);
	}
	else if (_type == COLLISION_BOX)
	{
		static_cast<CollisionBoxFile*>(this)->serialize(s);
	}
	else if (_type == COLLISION_SPHERE)
	{
		static_cast<CollisionSphereFile*>(this)->serialize(s);
	}
}

	void AMediaFile::saveToFile()
	{
		std::ofstream ofs(this->path.getFullName().c_str(), std::ios::binary);
		assert(ofs.is_open() && "Cannot open file for save");
		serialize<cereal::BinaryOutputArchive>(ofs);
	}
