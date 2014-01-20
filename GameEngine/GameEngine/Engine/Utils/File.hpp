#ifndef					__FILE_HPP__
# define				__FILE_HPP__

#include				<algorithm>
#include				<fstream>

class					File
{
public:
	File(const std::string &name = "unknownFile") :
		fullPath_(name)
	{
	}

	File(const char *name) :
		fullPath_(name)
	{
	}

	bool					exists() const
	{
		std::ifstream file(this->fullPath_.c_str());

		return file.is_open();
	}

	const std::string			&getFullName() const
	{
		return this->fullPath_;
	}

	std::string				getFileName() const
	{
		std::string::size_type		pos;

		pos = this->fullPath_.find_last_of("\\/");

		if (pos != std::string::npos)
			return this->fullPath_.substr(pos + 1, std::string::npos);
		else
			return this->fullPath_;
	}

	std::string             getFolder() const
	{
		std::string::size_type		pos;

		pos = this->fullPath_.find_last_of("\\/");
		if (pos != std::string::npos)
			return this->fullPath_.substr(0, pos + 1);
		else
			return this->fullPath_;
	}

	std::string				getShortFileName() const
	{
		return this->getFileName().substr(0, this->getFileName().find_last_of("."));
	}

	std::string				getExtension() const
	{
		std::string::size_type		pos;

		pos = this->fullPath_.find_last_of(".");
		if (pos != std::string::npos)
			return this->fullPath_.substr(pos + 1, std::string::npos);
		else
			return "";
	}

	std::string				getFileContent() const
	{
		std::ifstream			stream(fullPath_.c_str());

		if (!stream.is_open())
			return "";
		std::string				res;

		res.assign(( std::istreambuf_iterator<char>(stream) ),
			( std::istreambuf_iterator<char>()));

		return res;
	}
private:
	std::string				fullPath_;
};

#endif					// __FILE_HPP__
