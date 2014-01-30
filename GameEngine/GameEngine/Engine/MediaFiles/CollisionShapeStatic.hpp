#ifndef   __COLLISION_SHAPE_STATIC_HPP__
# define  __COLLISION_SHAPE_STATIC_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <Utils/File.hpp>
#include <btBulletDynamicsCommon.h>
#include <btBulletWorldImporter.h>

struct CollisionShapeStatic : public MediaFile<CollisionShapeStatic>
{
public:
	CollisionShapeStatic()
		: MediaFile<CollisionShapeStatic>()
		, shape(nullptr)
	{
		_type = COLLISION_SHAPE_STATIC;
	}

	virtual ~CollisionShapeStatic()
	{
	}

	CollisionShapeStatic(const CollisionShapeStatic &o)
		: MediaFile<CollisionShapeStatic>(o)
		, shape(nullptr)
	{
	}

	CollisionShapeStatic &operator=(const CollisionShapeStatic &o)
	{
		if (&o != this)
		{
			shape = o.shape;
		}
		return *this;
	}

	AMediaFile *unserialize(const File &f)
	{
		btBulletWorldImporter import(0);
		assert(f.exists() == true && "File do not exists");
		assert(import.loadFile(f.getFullName().c_str()) == true && "Bullet importer cannot open file.");
		int n = import.getNumCollisionShapes();
		assert(n > 0 && "Bullet file is not correct. No collision shape inside.");
		shape = std::shared_ptr<btBvhTriangleMeshShape>(static_cast<btBvhTriangleMeshShape*>(import.getCollisionShapeByIndex(0)));
	}

	// WILL NEVER BE CALLED
	template <class Archive>
	void serialize(Archive &ar)
	{}

	void serialize() const
	{
		btDefaultSerializer	serializer;
		serializer.startSerialization();
		shape->serializeSingleShape(&serializer);
		serializer.finishSerialization();
		FILE *f = fopen(path.getFullName().c_str(), "wb");
		fwrite(serializer.getBufferPointer(), serializer.getCurrentBufferSize(), 1, f);
		fclose(f);
	}
	std::shared_ptr<btBvhTriangleMeshShape> shape;
};

#endif    //__COLLISION_SHAPE_STATIC_HPP__