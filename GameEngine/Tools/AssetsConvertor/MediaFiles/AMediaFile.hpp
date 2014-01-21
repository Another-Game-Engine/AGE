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

struct AMediaFile
{
public:
	File path;
	std::string name;
	std::size_t type;
public:
	AMediaFile()
	{
	}
	virtual ~AMediaFile(){}

	template <typename Archive>
	void serialize(std::ofstream &s)
	{
		Archive ar(s);
		ar(type);
		_serialize(ar);
	}

	virtual AMediaFile *unserialize(cereal::JSONInputArchive &ar) = 0;
	virtual AMediaFile *unserialize(cereal::BinaryInputArchive &ar) = 0;
	virtual AMediaFile *unserialize(cereal::XMLInputArchive &ar) = 0;
	virtual AMediaFile *unserialize(cereal::PortableBinaryInputArchive &ar) = 0;
	virtual void _serialize(cereal::JSONOutputArchive &ar) = 0;
	virtual void _serialize(cereal::BinaryOutputArchive &ar) = 0;
	virtual void _serialize(cereal::XMLOutputArchive &ar) = 0;
	virtual void _serialize(cereal::PortableBinaryOutputArchive &ar) = 0;
};

#endif    //__AMEDIA_FILE_HPP__