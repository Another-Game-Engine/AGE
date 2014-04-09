#ifndef   __COLLISION_SPHERE_FILE_HPP__
# define  __COLLISION_SPHERE_FILE_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <Utils/File.hpp>
#include <btBulletDynamicsCommon.h>
#include <Serialize/BulletWorldImporter/btBulletWorldImporter.h>

struct CollisionSphereFile : public MediaFile<CollisionSphereFile>
{
public:
	CollisionSphereFile();
	~CollisionSphereFile();
	CollisionSphereFile(const CollisionSphereFile &o);
	CollisionSphereFile &operator=(const CollisionSphereFile &o);
	void unserialize(const File &f);
	void serialize(std::ofstream &s);
	void save(cereal::PortableBinaryOutputArchive &ar) const{};
	void load(cereal::PortableBinaryInputArchive &ar){};
	std::shared_ptr<btSphereShape> shape;
};

#endif    //__COLLISION_SPHERE_FILE_HPP__
