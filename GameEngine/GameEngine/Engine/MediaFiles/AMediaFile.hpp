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
	}

	AMediaFile &operator=(const AMediaFile &o)
	{
		if (&o != this)
		{
			_type = o._type;
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