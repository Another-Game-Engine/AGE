#include "CollisionSphereFile.hpp"

CollisionSphereFile::CollisionSphereFile()
: MediaFile<CollisionSphereFile>()
, shape(nullptr)
{
	_type = COLLISION_SPHERE;
}

CollisionSphereFile::~CollisionSphereFile()
{
}

CollisionSphereFile::CollisionSphereFile(const CollisionSphereFile &o)
: MediaFile<CollisionSphereFile>(o)
, shape(nullptr)
{
	shape = o.shape;
}

CollisionSphereFile &CollisionSphereFile::operator=(const CollisionSphereFile &o)
{
	if (&o != this)
	{
		shape = o.shape;
	}
	return *this;
}

void CollisionSphereFile::unserialize(const File &f)
{
	btBulletWorldImporter import(0);
	assert(f.exists() == true && "File do not exists");
	auto loadFile = import.loadFile(f.getFullName().c_str());
	assert(loadFile == true && "Bullet importer cannot open file.");
	int n = import.getNumCollisionShapes();
	assert(n > 0 && "Bullet file is not correct. No collision box inside.");
	auto o = import.getCollisionShapeByIndex(0);
	shape = std::shared_ptr<btSphereShape>(static_cast<btSphereShape*>(o));
}

void CollisionSphereFile::serialize(std::ofstream &s)
{
	btDefaultSerializer	serializer;
	serializer.startSerialization();
	shape->serializeSingleShape(&serializer);
	serializer.finishSerialization();
	s.write((const char *)(serializer.getBufferPointer()), serializer.getCurrentBufferSize());
}