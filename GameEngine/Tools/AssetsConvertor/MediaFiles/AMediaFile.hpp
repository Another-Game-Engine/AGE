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
#include <cereal/archives/binary.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>

struct AMediaFile
{
	AMediaFile();
	virtual ~AMediaFile();

	File path;
	std::string name;

	virtual void serialize(std::ofstream &s) = 0;
	virtual AMediaFile *unserialize(std::ifstream &s) = 0;
protected:
	static int uniqueId()
	{
		static int id = 0;
		return id++;
	}
};

template <typename T>
struct AMediaFileBase : public AMediaFile
{
	static unsigned int getTypeId()
	{
		static unsigned int id = AMediaFile::uniqueId();
		return id;
	}
};

#endif    //__AMEDIA_FILE_HPP__