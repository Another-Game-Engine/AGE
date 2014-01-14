#ifndef   __AMEDIA_FILE_HPP__
# define  __AMEDIA_FILE_HPP__

#include <Utils/File.hpp>
#include <Cereal/cereal.hpp>

struct AMediaFile
{
	File path;
	std::string name;
};

template <class Archive>
void serialize( Archive &ar, AMediaFile & c)
{
	ar( CEREAL_NVP(c.path), CEREAL_NVP(c.name) );
}

template <class Archive>
void serialize( Archive &ar, File & c)
{
	ar( CEREAL_NVP(c.getFullName()));
}


#endif    //__AMEDIA_FILE_HPP__