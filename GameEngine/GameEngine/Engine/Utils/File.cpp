#include "File.hpp"
#include <algorithm>
#include <fstream>

OldFile::OldFile(const std::string &name /*= "unknownFile"*/) :
fullPath_(name)
{
}

OldFile::OldFile(const char *name) :
fullPath_(name)
{
}

bool OldFile::exists() const
{
	std::ifstream file(this->fullPath_.c_str());
	bool isOpen = file.is_open();
	if (isOpen)
		file.close();
	return isOpen;
}

const std::string OldFile::getFullName() const
{
	return this->fullPath_;
}

std::string OldFile::getFileName() const
{
	std::string::size_type		pos;
	auto path = fullPath_;

	pos = path.find_last_of("\\/");

	if (pos != std::string::npos)
		return path.substr(pos + 1, std::string::npos);
	else
		return path;
}

std::string OldFile::getFolder() const
{
	std::string::size_type		pos;
	auto path = fullPath_;

	pos = path.find_last_of("\\/");
	if (pos != std::string::npos)
		return path.substr(0, pos + 1);
	else
		return "";
}

std::string OldFile::getShortFileName() const
{
	return this->getFileName().substr(0, this->getFileName().find_last_of("."));
}

std::string OldFile::getExtension() const
{
	std::string::size_type		pos;
	auto path = fullPath_;
	pos = path.find_last_of(".");
	if (pos != std::string::npos)
		return path.substr(pos + 1, std::string::npos);
	else
		return "";
}

std::string OldFile::getFileContent() const
{
	std::ifstream			stream(fullPath_.c_str());

	if (!stream.is_open())
		return "";
	std::string				res;

	res.assign((std::istreambuf_iterator<char>(stream)),
		(std::istreambuf_iterator<char>()));

	return res;
}