#ifndef   __AMEDIA_FILE_HPP__
# define  __AMEDIA_FILE_HPP__

#include <functional>
#include <Utils/File.hpp>

#include <cereal/cereal.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>

struct AMediaFile;

struct FileTypeRegister
{
	static FileTypeRegister *getInstance()
	{
		static FileTypeRegister *t = new FileTypeRegister();
		return t;
	}
	std::map<std::size_t, AMediaFile*> refs;

	void registerType(AMediaFile *t);
	AMediaFile *getFromType(std::size_t key, cereal::JSONInputArchive &s);
	AMediaFile *unserializeFromStream(std::ifstream &s);

	template <typename T>
	void registerType()
	{
		registerType(new T());
	}
};

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

	virtual void serialize(std::ofstream &s) = 0;
	virtual AMediaFile *unserialize(cereal::JSONInputArchive &ar) = 0;
	virtual AMediaFile *unserialize(cereal::BinaryInputArchive &ar) = 0;
	virtual AMediaFile *unserialize(cereal::XMLInputArchive &ar) = 0;
};

#endif    //__AMEDIA_FILE_HPP__