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
		bool isOpen = file.is_open();
		if (isOpen)
			file.close();
		return isOpen;
	}

	const std::string			getFullName() const
	{
		return this->fullPath_;
	}

	std::string				getFileName() const
	{
		std::string::size_type		pos;
		auto path = fullPath_;

		pos = path.find_last_of("\\/");

		if (pos != std::string::npos)
			return path.substr(pos + 1, std::string::npos);
		else
			return path;
	}

	std::string             getFolder() const
	{
		std::string::size_type		pos;
		auto path = fullPath_;

		pos = path.find_last_of("\\/");
		if (pos != std::string::npos)
			return path.substr(0, pos + 1);
		else
			return "";
	}

	std::string				getShortFileName() const
	{
		return this->getFileName().substr(0, this->getFileName().find_last_of("."));
	}

	std::string				getExtension() const
	{
		std::string::size_type		pos;
		auto path = fullPath_;
		pos = path.find_last_of(".");
		if (pos != std::string::npos)
			return path.substr(pos + 1, std::string::npos);
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
