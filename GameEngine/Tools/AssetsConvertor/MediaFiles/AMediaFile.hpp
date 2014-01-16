#ifndef   __AMEDIA_FILE_HPP__
# define  __AMEDIA_FILE_HPP__

#include <Utils/File.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

struct AMediaFile
{
	AMediaFile();
	virtual ~AMediaFile();
		virtual void test(){}
	File path;
	std::string name;

	template <class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & "lol";
    }
};

#endif    //__AMEDIA_FILE_HPP__