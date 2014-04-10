#pragma once

#include <Utils/Dependency.hpp>
#include <Utils/File.hpp>
#include <map>
#include <functional>

struct Configuration
{
	Configuration()
	: triggerCallback(false)
	{}

	virtual ~Configuration(){}
	bool triggerCallback;
};

template <typename T>
struct ConfigurationValue : public Configuration
{
	ConfigurationValue(const T &_value)
	: value(_value)
	{}

	ConfigurationValue(T && _value)
	: value(std::move(_value))
	{}

	T &getValue()
	{
		return value;
	}

	T &&getValue()
	{
		return std::move(value);
	}

	template <typename Archive>
	void save(Archive &ar) const
	{
		ar(value);
	}

	template <typename Archive>
	void load(Archive &ar)
	{
		ar(value);
	}

	T value;
	std::function<void(T &value)> callback;
};

class ConfigurationManager : public Dependency
{
public:
	ConfigurationManager()
		: _saveFile(File("./ConfigurationSaveFile.conf"))
	{
	}

	virtual ~ConfigurationManager()
	{}

	void setConfigurationFilePath(const File &file)
	{
		_saveFile = file;
	}

	template <typename ArchiveType>
	bool saveToFile()
	{
		return true;
	}

	template <typename ArchiveType>
	bool loadFile()
	{
		return true;
	}

	template <typename T>
	void setConfiguration(
		const std::string &name
		, const T &value)
	{
		if (_confs.find(name) != std::end(_confs))
			return;
		auto ptr = std::make_unique<ConfigurationValue<T>>(std::move(value));
		_confs.emplace(std::make_pair(name, ptr));
	}

	// with callback at modification
	template <typename T>
	void setConfiguration(
		const std::string &name
		, const T &value
		, std::function<void(const T &value)> &callback)
	{
		if (_confs.find(name) != std::end(_confs))
			return;
		auto ptr = std::make_unique<ConfigurationValue<T>>(std::move(value));
		ptr->triggerCallback = true;
		ptr->callback = callback;
		_confs.emplace(std::make_pair(name, ptr));
	}

private:
	File _saveFile;
	std::map<std::string, std::unique_ptr<Configuration>> _confs;
};