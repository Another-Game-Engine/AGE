#ifndef   __COLLISION_BOX_FILE_HPP__
# define  __COLLISION_BOX_FILE_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <Utils/File.hpp>
#include <btBulletDynamicsCommon.h>
#include <Serialize/BulletWorldImporter/btBulletWorldImporter.h>

struct CollisionBoxFile : public MediaFile<CollisionBoxFile>
{
public:
	CollisionBoxFile()
		: MediaFile<CollisionBoxFile>()
		, shape(nullptr)
	{
		_type = COLLISION_BOX;
	}

	virtual ~CollisionBoxFile()
	{
	}

	CollisionBoxFile(const CollisionBoxFile &o)
		: MediaFile<CollisionBoxFile>(o)
		, shape(nullptr)
	{
			shape = o.shape;
	}

	CollisionBoxFile &operator=(const CollisionBoxFile &o)
	{
		if (&o != this)
		{
			shape = o.shape;
		}
		return *this;
	}

	void unserialize(const File &f)
	{
		btBulletWorldImporter import(0);
		assert(f.exists() == true && "File do not exists");
		auto loadFile = import.loadFile(f.getFullName().c_str());
		assert(loadFile == true && "Bullet importer cannot open file.");
		int n = import.getNumCollisionShapes();
		assert(n > 0 && "Bullet file is not correct. No collision box inside.");
		auto o = import.getCollisionShapeByIndex(0);
		shape = std::shared_ptr<btBoxShape>(static_cast<btBoxShape*>(o));
	}

	// WILL NEVER BE CALLED
	template <class Archive>
	void serialize(Archive &ar)
	{}

	void serialize(std::ofstream &s)
	{
		btDefaultSerializer	serializer;
		serializer.startSerialization();
		shape->serializeSingleShape(&serializer);
		serializer.finishSerialization();
		s.write((const char *)(serializer.getBufferPointer()), serializer.getCurrentBufferSize());
	}

	std::shared_ptr<btBoxShape> shape;
};

#endif    //__COLLISION_BOX_FILE_HPP__