#pragma once

#include <string>

class MyMediaFile
{
public:
	MyMediaFile(std::string &&path, std::string &&name, std::string &&ext)
		: _path(std::move(path))
		, _name(std::move(name))
		, _extension(std::move(ext))
	{}

	virtual ~MyMediaFile()
	{}

	const std::string &&getPath() const
	{
		return std::move(_path);
	}

	const std::string &&getName() const
	{
		return std::move(_name);
	}

	const std::string &&getExtension() const
	{
		return std::move(_extension);
	}

	void setPath(std::string &&path)
	{
		_path = std::move(path);
	}

	void setName(std::string &&name)
	{
		_name = std::move(name);
	}

	void setExtension(std::string &&ext)
	{
		_extension = std::move(ext);
	}

	bool operator==(const MyMediaFile &&o)
	{
		return (_path == o._path && _name == o._name);
	}
private:
	std::string _path;
	std::string _name;
	std::string _extension;
};