#pragma once

#include <Utils/Dependency.hpp>
#include <Utils/File.hpp>
#include <map>
#include <functional>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>
#include <Utils/GlmSerialization.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>

struct Configuration
{
	Configuration(std::string &&_key)
	: triggerCallback(false)
	, key(_key)
	{}

	virtual void save(cereal::JSONOutputArchive &ar) const = 0;
	virtual void load(cereal::JSONOutputArchive &ar) = 0;

	virtual ~Configuration(){}
	bool triggerCallback;
	std::string key;
};

template <typename T>
struct ConfigurationValue : public Configuration
{
	ConfigurationValue()
	{}

	ConfigurationValue(std::string &&_key, const T &_value)
		: Configuration(std::move(_key)) 
		, value(_value)
	{}

	ConfigurationValue(std::string &&_key, T && _value)
		: Configuration(std::move(_key))
		, value(std::move(_value))
	{}

	T &getValue()
	{
		return value;
	}

	virtual void save(cereal::JSONOutputArchive &ar) const
	{
		ar(key, value);
	}

	virtual void load(cereal::JSONOutputArchive &ar)
	{
		ar(key, value);
	}

	T value;
	std::function<void(T &&value)> callback;
};

class ConfigurationManager : public Dependency
{
public:
	ConfigurationManager(const File &saveFile = File("./ConfigurationSaveFile.conf"))
		: _saveFile(saveFile)
	{
	}

	virtual ~ConfigurationManager()
	{}

	void setConfigurationFilePath(const File &file)
	{
		_saveFile = file;
	}

	bool saveToFile()
	{
		auto fileStream = std::ofstream(_saveFile.getFullName());
		if (!fileStream.is_open())
			return false;
		cereal::JSONOutputArchive ar(fileStream);
		for (auto &&e : _confs)
		{
			e.second->save(ar);
		}

		return true;
	}

	bool loadFile()
	{
		return true;
	}

	template <typename T>
	void setConfiguration(
		const std::string &&name
		, T &&value)
	{
		if (_confs.find(name) != std::end(_confs))
			return;
		auto ptr = std::make_unique<ConfigurationValue<T>>(std::move(name), std::move(value));
		_confs.emplace(std::make_pair(name, ptr));
	}

	// with callback at modification
	template <typename T>
	void setConfiguration(
		std::string &&name
		, T &&value
		, std::function<void(T &&v)> &&callback)
	{
		if (_confs.find(name) != std::end(_confs))
			return;
		auto ptr = std::make_unique<ConfigurationValue<T>>(std::move(name), std::move(value));
		ptr->triggerCallback = true;
		ptr->callback = callback;
		_confs.emplace(std::make_pair(name, std::move(ptr)));
	}

	template <typename T>
	void setValue(const std::string &name, T &&value)
	{
		auto it = _confs.find(name);
		if (it == std::end(_confs))
			return;
		auto ptr = dynamic_cast<ConfigurationValue<T>*>(it->second.get());
		if (!ptr)
			return;
		ptr->value = value;
		if (ptr->triggerCallback)
			ptr->callback(std::move(value));
	}

private:
	File _saveFile;
	std::map<std::string, std::unique_ptr<Configuration>> _confs;
};