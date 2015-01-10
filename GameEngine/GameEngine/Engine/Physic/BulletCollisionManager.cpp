#include "BulletCollisionManager.hpp"
#include <Utils/File.hpp>
#include <bullet/src/Serialize/BulletWorldImporter/btBulletWorldImporter.h>

std::shared_ptr<btBvhTriangleMeshShape> BulletCollisionManager::loadStaticShape(const std::string &path)
{
	File filePath(path);
	if (!filePath.exists())
	{
		std::cerr << "Bullet file not found" << std::endl;
		return nullptr;
	}
	if (_staticShapes.find(path) != std::end(_staticShapes))
		return _staticShapes[path];
	btBulletWorldImporter import(0);
	auto loadStatus = import.loadFile(path.c_str());
	if (loadStatus != true)
	{
		std::cerr << "Bullet importer cannot open file." << std::endl;
		return nullptr;
	}
	int n = import.getNumCollisionShapes();
	if (n <= 0)
	{
		std::cerr << "Bullet file is not correct. No collision box inside." << std::endl;
		return nullptr;
	}
	auto o = import.getCollisionShapeByIndex(0);
	auto shape = std::shared_ptr<btBvhTriangleMeshShape>(static_cast<btBvhTriangleMeshShape*>(o));
	_staticShapes.insert(std::make_pair(path, shape));
	return shape;
}