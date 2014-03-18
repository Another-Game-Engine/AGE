#include "CollisionBoxFile.hpp"

CollisionBoxFile::CollisionBoxFile()
: MediaFile<CollisionBoxFile>()
, shape(nullptr)
{
	_type = COLLISION_BOX;
}

CollisionBoxFile::~CollisionBoxFile()
{
}

CollisionBoxFile::CollisionBoxFile(const CollisionBoxFile &o)
: MediaFile<CollisionBoxFile>(o)
, shape(nullptr)
{
	shape = o.shape;
}

CollisionBoxFile &CollisionBoxFile::operator = (const CollisionBoxFile &o)
{
	if (&o != this)
	{
		shape = o.shape;
	}
	return *this;
}

void CollisionBoxFile::unserialize(const File &f)
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

void CollisionBoxFile::serialize(std::ofstream &s)
{
	btDefaultSerializer	serializer;
	serializer.startSerialization();
	shape->serializeSingleShape(&serializer);
	serializer.finishSerialization();
	s.write((const char *)(serializer.getBufferPointer()), serializer.getCurrentBufferSize());
}