#ifndef   __AMEDIA_FILE_HPP__
# define  __AMEDIA_FILE_HPP__

#include <Utils/File.hpp>
#include <Cereal/cereal.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/utility.hpp>

struct AMediaFile
{
	AMediaFile();
	virtual ~AMediaFile();

	File path;
	std::string name;

	template <class Archive>
	void serialize(Archive &ar)
	{
		unsigned int t = 2;
//		ar(CEREAL_NVP(c.name));
		ar(t);
	}
};

//template <class Archive>
//void serialize( Archive &ar, File & c)
//{
//	ar( CEREAL_NVP(c.getFullName()));
//}

#endif    //__AMEDIA_FILE_HPP__