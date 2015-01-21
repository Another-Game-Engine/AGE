#include "File.hpp"
#include <algorithm>
#include <fstream>

File::File(const std::string &name /*= "unknownFile"*/) :
fullPath_(name)
{
}

File::File(const char *name) :
fullPath_(name)
{
}

bool File::exists() const
{
	std::ifstream file(this->fullPath_.c_str());
	bool isOpen = file.is_open();
	if (isOpen)
		file.close();
	return isOpen;
}

const std::string File::getFullName() const
{
	return this->fullPath_;
}

std::string File::getFileName() const
{
	std::string::size_type		pos;
	auto path = fullPath_;

	pos = path.find_last_of("\\/");

	if (pos != std::string::npos)
		return path.substr(pos + 1, std::string::npos);
	else
		return path;
}

std::string File::getFolder() const
{
	std::string::size_type		pos;
	auto path = fullPath_;

	pos = path.find_last_of("\\/");
	if (pos != std::string::npos)
		return path.substr(0, pos + 1);
	else
		return "";
}

std::string File::getShortFileName() const
{
	return this->getFileName().substr(0, this->getFileName().find_last_of("."));
}

std::string File::getExtension() const
{
	std::string::size_type		pos;
	auto path = fullPath_;
	pos = path.find_last_of(".");
	if (pos != std::string::npos)
		return path.substr(pos + 1, std::string::npos);
	else
		return "";
}

std::string File::getFileContent() const
{
	std::ifstream			stream(fullPath_.c_str());

	if (!stream.is_open())
		return "";
	std::string				res;

	res.assign((std::istreambuf_iterator<char>(stream)),
		(std::istreambuf_iterator<char>()));

	return res;
}