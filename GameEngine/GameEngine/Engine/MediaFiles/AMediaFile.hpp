#ifndef   __AMEDIA_FILE_HPP__
# define  __AMEDIA_FILE_HPP__

#include <functional>
#include <Utils/File.hpp>

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
#include <MediaFiles/AssetsManager.hpp>

struct AMediaFile
{
public:
	File path;
	std::string name;
	static AssetsManager *_manager;
protected:
	std::size_t _childs;
	enum MEDIA_TYPE
	{
		UNKNOWN = 0
		, OBJ = 1
		, TEXTURE = 2
		, MATERIAL = 3
		, CUBEMAP = 4
	};
	MEDIA_TYPE _type;
public:
	AMediaFile() :
		_childs(0)
		, _type(UNKNOWN)
	{
	}
	virtual ~AMediaFile(){}

	AMediaFile(const AMediaFile &o)
		: _childs(0)
	{
		_type = o._type;
		name = o.name;
		name += "_copy";
		auto tmpPath = o.path.getFolder() + o.path.getShortFileName();
		tmpPath += "_copy.cpd";
		path = File(tmpPath);
	}

	AMediaFile &operator=(const AMediaFile &o)
	{
		if (&o != this)
		{
			_type = o._type;
			name = o.name;
			name += "_copy";
		}
		return *this;
	}

	template <typename Archive>
	void serialize(std::ofstream &s)
	{
		Archive ar(s);
		ar(_type);
		_serialize(ar);
	}

	template <typename Archive>
	static std::shared_ptr<AMediaFile> loadFromFile(const File &file)
	{
		assert(file.exists() == true && "File does not exist.");
		MEDIA_TYPE serializedFileType = UNKNOWN;
		std::shared_ptr<AMediaFile> res{ nullptr };
		res = _manager->get(file.getShortFileName());
		if (res != nullptr)
			return res;

		std::ifstream ifs(file.getFullName(), std::ios::binary);
		Archive ar(ifs);
		ar(serializedFileType);
		switch (serializedFileType)
		{
		case OBJ:
			res = std::make_shared<ObjFile>();
			ar(static_cast<ObjFile&>(*res.get()));
			break;
		case MATERIAL:
			res = std::make_shared<MaterialFile>();
			ar(static_cast<MaterialFile&>(*res.get()));
			break;
		case TEXTURE:
			res = std::make_shared<TextureFile>();
			ar(static_cast<TextureFile&>(*res.get()));
			break;
		case CUBEMAP:
			res = std::make_shared<CubeMapFile>();
			ar(static_cast<CubeMapFile&>(*res.get()));
			break;
		default:
			break;
		}
		assert(res != nullptr && "Unknown MediaFile type.");
		assert(_manager != nullptr && "Media Manager is not set.");
		res->path = file.getFullName();
		res->name = file.getShortFileName();
		_manager->add(res);
		return res;
	}

	static void loadFromList(const File &file);
	static std::shared_ptr<AMediaFile> get(const std::string &name);

	template <class T>
	static std::shared_ptr<T> get(const std::string &name)
	{
		return std::static_pointer_cast<T>(_manager->get(name));
	}

	template <class T>
	static std::shared_ptr<T> create(const std::string &name = "", std::shared_ptr<AMediaFile> copyFrom = nullptr)
	{
		if (!name.empty())
		{
			auto o = std::static_pointer_cast<T>(_manager->get(name));
			if (o != nullptr)
				return o;
		}
		std::shared_ptr<T> n{ nullptr };
		if (copyFrom != nullptr)
			n = std::make_shared<T>(*std::static_pointer_cast<const T>(copyFrom).get());
		else
			n = std::make_shared<T>();
		if (!name.empty())
			n->name = name;
		_manager->add(n);
		return n;
	}

	static void saveToFile(const std::string &media, const std::string &path)
	{
		auto &e = get<AMediaFile>(media);
		assert(e != nullptr && "Media does not exists");
		std::string filePath = path + e->name + ".cpd";
		std::ofstream ofs(filePath, std::ios::binary);
		assert(ofs.is_open() && "Cannot open file for save");
		e->serialize<cereal::BinaryOutputArchive>(ofs);
	}

	static void setManager(AssetsManager *manager)
	{
		_manager = manager;
	}

	inline std::size_t getChilds() const
	{
		return _childs;
	}

	inline void incrementChilds()
	{
		++_childs;
	}

	inline std::size_t getType() const
	{
		return _type;
	}

protected:
	virtual void _serialize(cereal::JSONOutputArchive &ar) = 0;
	virtual void _serialize(cereal::BinaryOutputArchive &ar) = 0;
	virtual void _serialize(cereal::XMLOutputArchive &ar) = 0;
	virtual void _serialize(cereal::PortableBinaryOutputArchive &ar) = 0;
};

#endif    //__AMEDIA_FILE_HPP__