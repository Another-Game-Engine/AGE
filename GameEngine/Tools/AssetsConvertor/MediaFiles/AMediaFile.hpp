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

struct FileTypeRegister
{
	static FileTypeRegister *getInstance()
	{
		static FileTypeRegister *t = new FileTypeRegister();
		return t;
	}
	std::map<std::size_t, AMediaFile*> refs;


template <typename Archive>
AMediaFile *getFromType(std::size_t key, Archive &s)
{
	auto &it = refs.find(key);
	if (it == std::end(refs))
		return nullptr;
	return it->second->unserialize(s);
}

template <typename Archive>
AMediaFile *unserializeFromStream(std::ifstream &s)
{
	std::size_t key = 0;
	Archive ar(s);
	ar(key);
	return getFromType(key, ar);
}

	template <typename T>
	void registerType()
	{
		auto instance = new T();
		std::size_t key = instance->type;
		if (refs.find(key) != std::end(refs))
			return;
		refs.insert(std::make_pair(key, instance));
	}
};

#endif    //__AMEDIA_FILE_HPP__