#include "CollisionShapeStaticFile.hpp"

CollisionShapeStaticFile::CollisionShapeStaticFile()
: MediaFile<CollisionShapeStaticFile>()
, shape(nullptr)
{
	_type = COLLISION_SHAPE_STATIC;
}

CollisionShapeStaticFile::~CollisionShapeStaticFile()
{
}

CollisionShapeStaticFile::CollisionShapeStaticFile(const CollisionShapeStaticFile &o)
: MediaFile<CollisionShapeStaticFile>(o)
, shape(nullptr)
, trimesh(nullptr)
{
	shape = o.shape;
	trimesh = o.trimesh;
	////		shape = std::shared_ptr<btBvhTriangleMeshShape>(new btBvhTriangleMeshShape(*o.shape.get()));
	//		trimesh = std::shared_ptr<btTriangleMesh>(new btTriangleMesh(*o.shape->tri));
	//		std::shared_ptr<btBvhTriangleMeshShape>(new btBvhTriangleMeshShape(trimesh.get(), true));
}

CollisionShapeStaticFile &CollisionShapeStaticFile::operator=(const CollisionShapeStaticFile &o)
{
	if (&o != this)
	{
		shape = o.shape;
		trimesh = o.trimesh;
		//shape = std::shared_ptr<btBvhTriangleMeshShape>(new btBvhTriangleMeshShape(*o.shape.get()));
		//trimesh = std::shared_ptr<btTriangleMesh>(new btTriangleMesh(*o.trimesh.get()));
	}
	return *this;
}

void CollisionShapeStaticFile::unserialize(const File &f)
{
	btBulletWorldImporter import(0);
	assert(f.exists() == true && "File do not exists");
	auto loadFile = import.loadFile(f.getFullName().c_str());
	assert(loadFile == true && "Bullet importer cannot open file.");
	int n = import.getNumCollisionShapes();
	assert(n > 0 && "Bullet file is not correct. No collision box inside.");
	auto o = import.getCollisionShapeByIndex(0);
	shape = std::shared_ptr<btBvhTriangleMeshShape>(static_cast<btBvhTriangleMeshShape*>(o));
}


void CollisionShapeStaticFile::serialize(std::ofstream &s)
{
	btDefaultSerializer	serializer;
	serializer.startSerialization();
	shape->serializeSingleShape(&serializer);
	serializer.finishSerialization();
	s.write((const char *)(serializer.getBufferPointer()), serializer.getCurrentBufferSize());
}