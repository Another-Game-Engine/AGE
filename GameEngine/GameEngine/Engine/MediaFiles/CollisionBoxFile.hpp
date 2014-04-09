#ifndef   __COLLISION_BOX_FILE_HPP__
# define  __COLLISION_BOX_FILE_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <Utils/File.hpp>
#include <btBulletDynamicsCommon.h>
#include <Serialize/BulletWorldImporter/btBulletWorldImporter.h>

struct CollisionBoxFile : public MediaFile<CollisionBoxFile>
{
public:
	CollisionBoxFile();
	virtual ~CollisionBoxFile();
	CollisionBoxFile(const CollisionBoxFile &o);
	CollisionBoxFile &operator=(const CollisionBoxFile &o);
	void unserialize(const File &f);
	// WILL NEVER BE CALLED
	void serialize(std::ofstream &s);
	std::shared_ptr<btBoxShape> shape;
	void save(cereal::PortableBinaryOutputArchive &ar) const{};
	void load(cereal::PortableBinaryInputArchive &ar){};
};

#endif    //__COLLISION_BOX_FILE_HPP__
