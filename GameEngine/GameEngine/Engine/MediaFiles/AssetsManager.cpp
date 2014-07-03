#include <MediaFiles/AssetsManager.hpp>
#include <Core/Engine.hh>
#include <mediafiles/amediafile.hpp>
#include <mediafiles/collisionboxfile.hpp>
#include <mediafiles/collisionshapedynamicfile.hpp>
#include <mediafiles/collisionshapestaticfile.hpp>
#include <mediafiles/collisionspherefile.hpp>
#include <mediafiles/cubemapfile.hpp>
#include <mediafiles/materialfile.hpp>
#include <mediafiles/objfile.hpp>
#include <mediafiles/texturefile.hpp>

AssetsManager::AssetsManager()
{
}

AssetsManager::~AssetsManager()
{
}

void AssetsManager::init()
{
	// TODO DELETE THIS LINE AND AMEDIAFILE MANAGER PTR
	//AMediaFile::setManager(shared_from_this());
}

void AssetsManager::add(std::shared_ptr<AMediaFile> f)
{
	if (_files.find(f->name) != std::end(_files))
		return;
	_files.insert(std::make_pair(f->name, f));
}

std::shared_ptr<AMediaFile> AssetsManager::loadFromFile(const File &file)
{
	if (file.getExtension() == "bullet")
		return loadBulletFile(file);
	assert(file.exists() == true && "File does not exist.");
	std::shared_ptr<AMediaFile> res{ nullptr };
	res = get(file.getShortFileName());
	if (res != nullptr)
		return res;
	AMediaFile::MEDIA_TYPE serializedFileType = AMediaFile::UNKNOWN;

	std::ifstream ifs(file.getFullName(), std::ios::binary);
	cereal::PortableBinaryInputArchive ar(ifs);
	ar(serializedFileType);
	switch (serializedFileType)
	{
	case AMediaFile::OBJ:
		res = std::make_shared<ObjFile>();
		res->_dpyManager = _dpyManager;
		ar(static_cast<ObjFile&>(*res.get()));
		break;
	case AMediaFile::MATERIAL:
		res = std::make_shared<MaterialFile>();
		res->_dpyManager = _dpyManager;
		ar(static_cast<MaterialFile&>(*res.get()));
		break;
	case AMediaFile::TEXTURE:
		res = std::make_shared<TextureFile>();
		res->_dpyManager = _dpyManager;
		ar(static_cast<TextureFile&>(*res.get()));
		break;
	case AMediaFile::CUBEMAP:
		res = std::make_shared<CubeMapFile>();
		res->_dpyManager = _dpyManager;
		ar(static_cast<CubeMapFile&>(*res.get()));
		break;
	default:
		break;
	}
	assert(res != nullptr && "Unknown MediaFile type.");
	res->path = file;
	res->name = file.getShortFileName();
	add(res);
	return res;
}

void AssetsManager::loadFromList(const File &file)
{
	assert(file.exists() == true && "File does not exist.");

	std::ifstream ifs(file.getFullName(), std::ios::binary);
	cereal::JSONInputArchive ar(ifs);
	std::multimap<std::size_t, std::string> list;
	ar(list);

	for (auto &e : list)
	{
		loadFromFile(File(e.second));
	}
}

std::shared_ptr<AMediaFile> AssetsManager::loadBulletFile(const File &file)
{
	assert(file.exists() == true && "File does not exist.");
	std::shared_ptr<AMediaFile> res{ nullptr };
	res = get(file.getShortFileName());
	if (res != nullptr)
		return res;
	if (file.getShortFileName().find("collision_shape_static") != std::string::npos)
	{
		res = std::make_shared<CollisionShapeStaticFile>();
		res->_dpyManager = _dpyManager;
		static_cast<CollisionShapeStaticFile&>(*res.get()).unserialize(file);
	}
	else if (file.getShortFileName().find("collision_shape_dynamic") != std::string::npos)
	{
		res = std::make_shared<CollisionShapeDynamicFile>();
		res->_dpyManager = _dpyManager;
		static_cast<CollisionShapeDynamicFile&>(*res.get()).unserialize(file);
	}
	else if (file.getShortFileName().find("collision_sphere") != std::string::npos)
	{
		res = std::make_shared<CollisionSphereFile>();
		res->_dpyManager = _dpyManager;
		auto uuu = std::dynamic_pointer_cast<CollisionSphereFile>(res);
		uuu->unserialize(file);
	}
	else if (file.getShortFileName().find("collision_box") != std::string::npos)
	{
		res = std::make_shared<CollisionBoxFile>();
		res->_dpyManager = _dpyManager;
		static_cast<CollisionBoxFile&>(*res.get()).unserialize(file);
	}
	assert(res != nullptr && "Unknown MediaFile type.");
	res->path = File(file.getFullName());
	res->name = file.getShortFileName();
	add(res);
	return res;
}