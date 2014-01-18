#ifndef   __AMEDIA_FILE_HPP__
# define  __AMEDIA_FILE_HPP__

#include <Utils/File.hpp>

#include <cereal/cereal.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>

struct Basebase
{
	Basebase(){}
	virtual ~Basebase(){}
	template <class Archive>
	void serialize(Archive & ar)
	{
		int b = 3;
		ar(CEREAL_NVP(b));
    }
};

struct AMediaFile : public Basebase
{
	AMediaFile();
	virtual ~AMediaFile();
	virtual void test(){}
	File path;
	std::string name;

	template <class Archive>
	void serialize(Archive & ar)
	{
		int u = 2;
		ar(CEREAL_NVP(u));
    }
};

#endif    //__AMEDIA_FILE_HPP__