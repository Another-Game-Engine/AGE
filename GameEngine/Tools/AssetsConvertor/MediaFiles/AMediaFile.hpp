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
#include <Managers/AssetsManager.hpp>

struct AMediaFile
{
public:
	File path;
	std::string name;
	static AssetsManager *_manager;
protected:
	std::size_t _type;
	std::size_t _childs;
	enum MEDIA_TYPE
	{
		OBJ = 0,
		TEXTURE = 1,
		MATERIAL = 2,
		UNKNOWN = 3
	};
public:
	AMediaFile() :
		_childs(0)
	{
	}
	virtual ~AMediaFile(){}

	template <typename Archive>
	void serialize(std::ofstream &s)
	{
		Archive ar(s);
		ar(_type);
		_serialize(ar);
	}

	template <typename Archive>
	static std::shared_ptr<AMediaFile> loadFromFile(const File &file);

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