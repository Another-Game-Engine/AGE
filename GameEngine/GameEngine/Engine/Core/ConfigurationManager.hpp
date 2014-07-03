#pragma once

#include <Utils/Dependency.hpp>
#include <Utils/File.hpp>
#include <map>
#include <functional>
#include <Utils/JsonSerialization.hpp>
#include <cereal/external/rapidjson/stringbuffer.h>
#include <cereal/external/rapidjson/prettywriter.h>

struct Configuration
{
	Configuration(const std::string &_key = "NULL")
	: triggerCallback(false)
	, key(_key)
	{}

	void save(rapidjson::Value &json, rapidjson::Document &document) const
	{
		_save(json, document);
	}

	void load(rapidjson::Value &json, rapidjson::Document &document)
	{
		_load(json, document);
	}

	virtual ~Configuration(){}
	bool triggerCallback;
	std::string key;

private:
	virtual void _save(rapidjson::Value &json, rapidjson::Document &document) const = 0;
	virtual void _load(rapidjson::Value &json, rapidjson::Document &document) = 0;

};

template <typename T>
struct ConfigurationValue : public Configuration
{
	ConfigurationValue()
	{}

	ConfigurationValue(const std::string &_key, const T &_value)
		: Configuration(_key) 
		, value(_value)
	{}

	T &getValue()
	{
		return value;
	}

	virtual void _save(rapidjson::Value &json, rapidjson::Document &document) const
	{
		JsonSerialization::save<T>(value, json, document);
	}

	virtual void _load(rapidjson::Value &json, rapidjson::Document &document)
	{
		JsonSerialization::load<T>(value, json, document);
		if (triggerCallback)
			callback(value);
	}

	T value;
	std::function<void(const T &value)> callback;
};

class ConfigurationManager : public Dependency<ConfigurationManager>
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
		rapidjson::Document document;
		document.SetObject();
		for (auto &&e : _confs)
		{
			rapidjson::Value v(rapidjson::kObjectType);
			e.second->save(v, document);
			document.AddMember(e.first.c_str(), v, document.GetAllocator());
		}
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);
		auto file = std::ofstream(_saveFile.getFullName());
		file << buffer.GetString();
		return true;
	}

	bool loadFile()
	{
		if (!_saveFile.exists())
			return false;
		auto content = _saveFile.getFileContent();

		rapidjson::Document document;
		document.Parse<0>(content.c_str());

		if (!document.IsObject())
			return false;

		for (auto &&e : _confs)
		{
			if (document.HasMember(e.first.c_str()))
			{
				e.second->load(document[e.first.c_str()], document);
			}
		}
		return true;
	}

	template <typename T>
	void setConfiguration(
		std::string &name
		, const T &value)
	{
		if (_confs.find(name) != std::end(_confs))
			return;
		auto ptr = std::make_unique<ConfigurationValue<T>>(name, value);
		_confs.emplace(std::make_pair(name, std::move(ptr)));
	}

	// with callback at modification
	template <typename T>
	void setConfiguration(
		const std::string &name
		, const T &value
		, const std::function<void(const T &v)> callback)
	{
		if (_confs.find(name) != std::end(_confs))
			return;
		auto ptr = std::make_unique<ConfigurationValue<T>>(name, value);
		ptr->triggerCallback = true;
		ptr->callback = callback;
		_confs.emplace(std::make_pair(name, std::move(ptr)));
		callback(value);
	}

	template <typename T>
	void setValue(const std::string &name, const T &value)
	{
		auto it = _confs.find(name);
		if (it == std::end(_confs))
			return;
		auto ptr = dynamic_cast<ConfigurationValue<T>*>(it->second.get());
		if (!ptr)
			return;
		ptr->value = value;
		if (ptr->triggerCallback)
			ptr->callback(value);
	}

private:
	File _saveFile;
	std::map<std::string, std::unique_ptr<Configuration>> _confs;
};