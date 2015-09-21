#pragma once

#include <Utils/Dependency.hpp>
#include <Utils/OldFile.hpp>
#include <map>
#include <functional>
#include <Utils/JsonSerialization.hpp>
#include <cereal/external/rapidjson/stringbuffer.h>
#include <cereal/external/rapidjson/prettywriter.h>

namespace AGE
{
	struct Configuration
	{
		Configuration(const std::string &_key = "NULL");
		void save(rapidjson::Value &json, rapidjson::Document &document) const;
		void load(rapidjson::Value &json, rapidjson::Document &document);
		virtual ~Configuration();

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

	class ConfigurationManager : public Dependency < ConfigurationManager >
	{
	public:
		ConfigurationManager(const OldFile &saveFile = OldFile("./ConfigurationSaveFile.conf"));
		virtual ~ConfigurationManager();
		void setConfigurationFilePath(const OldFile &file);
		bool saveToFile();
		bool loadFile();

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
			auto ptr = (ConfigurationValue<T>*)(it->second.get());
			if (!ptr)
				return;
			ptr->value = value;
			if (ptr->triggerCallback)
				ptr->callback(value);
		}

		template <typename T>
		ConfigurationValue<T> *getConfiguration(const std::string &name)
		{
			if (_confs.find(name) == std::end(_confs))
				return nullptr;
			return (ConfigurationValue<T>*)(_confs[name].get());
		}

	private:
		OldFile _saveFile;
		std::map<std::string, std::unique_ptr<Configuration>> _confs;
	};
}