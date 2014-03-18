#ifndef   __COLLISION_SHAPE_DYNAMIC_HPP__
# define  __COLLISION_SHAPE_DYNAMIC_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <Utils/File.hpp>
#include <btBulletDynamicsCommon.h>
#include <Serialize/BulletWorldImporter/btBulletWorldImporter.h>

struct CollisionShapeDynamicFile : public MediaFile<CollisionShapeDynamicFile>
{
public:
	CollisionShapeDynamicFile();

	virtual ~CollisionShapeDynamicFile();

	CollisionShapeDynamicFile(const CollisionShapeDynamicFile &o);

	CollisionShapeDynamicFile &operator=(const CollisionShapeDynamicFile &o);
	void unserialize(const File &f);
	void serialize(std::ofstream &s);
	void save(cereal::PortableBinaryOutputArchive &ar) const{};
	void load(cereal::PortableBinaryInputArchive &ar){};
	std::shared_ptr<btConvexHullShape> shape;
};

#endif    //__COLLISION_SHAPE_DYNAMIC_HPP__