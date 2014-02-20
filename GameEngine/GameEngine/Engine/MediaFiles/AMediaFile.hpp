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

class AssetsManager;

struct AMediaFile
{
public:
	File path;
	std::string name;
	static std::shared_ptr<AssetsManager> _manager;
	enum MEDIA_TYPE
	{
		UNKNOWN = 0
		, OBJ = 1
		, TEXTURE = 2
		, MATERIAL = 3
		, CUBEMAP = 4
		, COLLISION_SHAPE_STATIC = 5
		, COLLISION_SHAPE_DYNAMIC = 6
		, COLLISION_BOX = 7
		, COLLISION_SPHERE = 8
		, COLLISION_MULTI_SPHERE = 9
	};
protected:
	std::size_t _childs;
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
		if (_type >= COLLISION_SHAPE_STATIC && _type <= COLLISION_MULTI_SPHERE)
		{
			serializeAsBulletFile(s);
			return;
		}
		else
		{
			Archive ar(s);
			ar(_type);
			_serialize(ar);
		}
	}

	void serializeAsBulletFile(std::ofstream &s);

	void saveToFile();

	static void setManager(std::shared_ptr<AssetsManager> manager)
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

	inline MEDIA_TYPE getType() const
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