#ifndef   __ASSETS_MANAGER_HPP__
# define  __ASSETS_MANAGER_HPP__

#include <Utils/Dependency.hpp>
#include <Utils/File.hpp>
#include <memory>
#include <map>
#include <MediaFiles/AMediaFile.hpp>
#include <MediaFiles/CollisionBoxFile.hpp>
#include <MediaFiles/CollisionShapeDynamicFile.hpp>
#include <MediaFiles/CollisionShapeStaticFile.hpp>
#include <MediaFiles/CollisionSphereFile.hpp>
#include <MediaFiles/CubeMapFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <MediaFiles/ObjFile.hpp>
#include <MediaFiles/TextureFile.hpp>

#include <cereal/cereal.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/portable_binary.hpp>

class AssetsManager : public Dependency, public std::enable_shared_from_this<AssetsManager>
{
public:
	AssetsManager();
	virtual ~AssetsManager();
	virtual void init();
	void add(std::shared_ptr<AMediaFile> f);

	template <typename T = AMediaFile>
	std::shared_ptr<T> getFromFile(const File &file)
	{
		std::string name = file.getShortFileName();
		auto it = _files.find(name);
		if (it == std::end(_files))
			return std::static_pointer_cast<T>(loadFromFile<cereal::BinaryInputArchive>(file));
		return std::static_pointer_cast<T>(it->second);
	}

	template <typename T = AMediaFile>
	std::shared_ptr<T> get(const std::string &name)
	{
		auto it = _files.find(name);
		if (it == std::end(_files))
			return std::shared_ptr<T>{nullptr};
		return std::static_pointer_cast<T>(it->second);
	}

	template <typename Archive>
	std::shared_ptr<AMediaFile> loadFromFile(const File &file)
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
		Archive ar(ifs);
		ar(serializedFileType);
		switch (serializedFileType)
		{
		case AMediaFile::OBJ:
			res = std::make_shared<ObjFile>();
			ar(static_cast<ObjFile&>(*res.get()));
			break;
		case AMediaFile::MATERIAL:
			res = std::make_shared<MaterialFile>();
			ar(static_cast<MaterialFile&>(*res.get()));
			break;
		case AMediaFile::TEXTURE:
			res = std::make_shared<TextureFile>();
			ar(static_cast<TextureFile&>(*res.get()));
			break;
		case AMediaFile::CUBEMAP:
			res = std::make_shared<CubeMapFile>();
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

	void loadFromList(const File &file)
	{
		assert(file.exists() == true && "File does not exist.");

		std::ifstream ifs(file.getFullName(), std::ios::binary);
		cereal::JSONInputArchive ar(ifs);
		std::multimap<std::size_t, std::string> list;
		ar(list);

		for (auto &e : list)
		{
			loadFromFile<cereal::BinaryInputArchive>(File(e.second));
		}
	}

	std::shared_ptr<AMediaFile> loadBulletFile(const File &file)
	{
		assert(file.exists() == true && "File does not exist.");
		std::shared_ptr<AMediaFile> res{ nullptr };
		res = get(file.getShortFileName());
		if (res != nullptr)
			return res;
		if (file.getShortFileName().find("collision_shape_static") != std::string::npos)
		{
			res = std::make_shared<CollisionShapeStaticFile>();
			static_cast<CollisionShapeStaticFile&>(*res.get()).unserialize(file);
		}
		else if (file.getShortFileName().find("collision_shape_dynamic") != std::string::npos)
		{
			res = std::make_shared<CollisionShapeDynamicFile>();
			static_cast<CollisionShapeDynamicFile&>(*res.get()).unserialize(file);
		}
		else if (file.getShortFileName().find("collision_sphere") != std::string::npos)
		{
			res = std::make_shared<CollisionSphereFile>();
			auto uuu = std::dynamic_pointer_cast<CollisionSphereFile>(res);
			uuu->unserialize(file);
		}
		else if (file.getShortFileName().find("collision_box") != std::string::npos)
		{
			res = std::make_shared<CollisionBoxFile>();
			static_cast<CollisionBoxFile&>(*res.get()).unserialize(file);
		}
		assert(res != nullptr && "Unknown MediaFile type.");
		res->path = File(file.getFullName());
		res->name = file.getShortFileName();
		add(res);
		return res;
	}

	template <class T>
	std::shared_ptr<T> create(const std::string &name = "", std::shared_ptr<AMediaFile> copyFrom = nullptr)
	{
		if (!name.empty())
		{
			auto o = std::static_pointer_cast<T>(get(name));
			if (o != nullptr)
				return o;
		}
		std::shared_ptr<T> n{ nullptr };
		if (copyFrom != nullptr)
			n = std::make_shared<T>(*std::static_pointer_cast<const T>(copyFrom).get());
		else
			n = std::make_shared<T>();
		if (!name.empty())
		{
			n->name = name;
			n->path = File(n->path.getFolder() + name + "." + n->path.getExtension());
		}
		add(n);
		return n;
	}


protected:
	std::map<std::string, std::shared_ptr<AMediaFile>> _files;
};

#endif    //__ASSETS_MANAGER_HPP__