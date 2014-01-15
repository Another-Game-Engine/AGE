#ifndef   __AMEDIA_FILE_HPP__
# define  __AMEDIA_FILE_HPP__

#include <Utils/File.hpp>
#include <Utils/Archive.hpp>

struct AMediaFile
{
	AMediaFile();
	virtual ~AMediaFile();

	File path;
	std::string name;

	void serialize(std::ofstream &os)
	{
		Archive::serialize(os, path.getFullName());
		Archive::serialize(os, name);
		_serialize(os);
	}

	void unserialize(std::ifstream &is)
	{
		std::string pathName;
		Archive::unserialize(is, pathName);
		path = File(pathName);
		Archive::unserialize(is, name);
		_unserialize(is);
	}

private:
	virtual void _serialize(std::ofstream &os) = 0;
	virtual void _unserialize(std::ifstream &is) = 0;
};

#endif    //__AMEDIA_FILE_HPP__