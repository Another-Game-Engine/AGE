#include "CollisionShapeDynamicFile.hpp"

CollisionShapeDynamicFile::CollisionShapeDynamicFile()
: MediaFile<CollisionShapeDynamicFile>()
, shape(nullptr)
{
	_type = COLLISION_SHAPE_DYNAMIC;
}

CollisionShapeDynamicFile::~CollisionShapeDynamicFile()
{
}

CollisionShapeDynamicFile::CollisionShapeDynamicFile(const CollisionShapeDynamicFile &o)
: MediaFile<CollisionShapeDynamicFile>(o)
, shape(nullptr)
{
	shape = o.shape;
}

CollisionShapeDynamicFile &CollisionShapeDynamicFile::operator=(const CollisionShapeDynamicFile &o)
{
	if (&o != this)
	{
		shape = o.shape;
	}
	return *this;
}

void CollisionShapeDynamicFile::unserialize(const File &f)
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

void CollisionShapeDynamicFile::serialize(std::ofstream &s)
{
	btDefaultSerializer	serializer;
	serializer.startSerialization();
	shape->serializeSingleShape(&serializer);
	serializer.finishSerialization();
	s.write((const char *)(serializer.getBufferPointer()), serializer.getCurrentBufferSize());
}