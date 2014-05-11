#ifndef   __COLLISION_SHAPE_STATIC_HPP__
# define  __COLLISION_SHAPE_STATIC_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <Utils/File.hpp>
#include <btBulletDynamicsCommon.h>
#include <Serialize/BulletWorldImporter/btBulletWorldImporter.h>

struct CollisionShapeStaticFile : public MediaFile<CollisionShapeStaticFile>
{
public:
	CollisionShapeStaticFile();

	virtual ~CollisionShapeStaticFile();

	CollisionShapeStaticFile(const CollisionShapeStaticFile &o);

	CollisionShapeStaticFile &operator=(const CollisionShapeStaticFile &o);

	void unserialize(const File &f);
	void serialize(std::ofstream &s);
	void save(cereal::PortableBinaryOutputArchive &ar) const{};
	void load(cereal::PortableBinaryInputArchive &ar){};
	std::shared_ptr<btBvhTriangleMeshShape> shape;
	std::shared_ptr<btTriangleMesh> trimesh;
};

#endif    //__COLLISION_SHAPE_STATIC_HPP__
