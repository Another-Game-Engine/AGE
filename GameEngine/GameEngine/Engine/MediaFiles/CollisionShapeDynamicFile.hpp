#ifndef   __COLLISION_SHAPE_DYNAMIC_HPP__
# define  __COLLISION_SHAPE_DYNAMIC_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <Utils/File.hpp>
#include <btBulletDynamicsCommon.h>
#include <Serialize/BulletWorldImporter/btBulletWorldImporter.h>

struct CollisionShapeDynamicFile : public MediaFile<CollisionShapeDynamicFile>
{
public:
	CollisionShapeDynamicFile()
		: MediaFile<CollisionShapeDynamicFile>()
		, shape(nullptr)
	{
		_type = COLLISION_SHAPE_DYNAMIC;
	}

	virtual ~CollisionShapeDynamicFile()
	{
	}

	CollisionShapeDynamicFile(const CollisionShapeDynamicFile &o)
		: MediaFile<CollisionShapeDynamicFile>(o)
		, shape(nullptr)
	{
			shape = o.shape;
	}

	CollisionShapeDynamicFile &operator=(const CollisionShapeDynamicFile &o)
	{
		if (&o != this)
		{
			shape = o.shape;
		}
		return *this;
	}

	void unserialize(File f)
	{
		btBulletWorldImporter import(0);
		assert(f.exists() == true && "File do not exists");
		auto loadFile = import.loadFile(f.getFullName().c_str());
		assert(loadFile == true && "Bullet importer cannot open file.");
		int n = import.getNumCollisionShapes();
		assert(n > 0 && "Bullet file is not correct. No collision box inside.");
		auto o = import.getCollisionShapeByIndex(0);
		shape = std::shared_ptr<btConvexHullShape>(static_cast<btConvexHullShape*>(o));
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

	std::shared_ptr<btConvexHullShape> shape;
};

#endif    //__COLLISION_SHAPE_DYNAMIC_HPP__