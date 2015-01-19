#include "ConfigurationManager.hpp"

namespace AGE
{
	Configuration::Configuration(const std::string &_key/* = "NULL"*/)
		: triggerCallback(false)
		, key(_key)
	{}

	void Configuration::save(rapidjson::Value &json, rapidjson::Document &document) const
	{
		_save(json, document);
	}

	void Configuration::load(rapidjson::Value &json, rapidjson::Document &document)
	{
		_load(json, document);
	}

	Configuration::~Configuration(){}

	/////////////////////////////////////////////////////////////////////////////////////////

	ConfigurationManager::ConfigurationManager(const File &saveFile /*= File("./ConfigurationSaveFile.conf")*/)
		: _saveFile(saveFile)
	{
	}

	ConfigurationManager::~ConfigurationManager()
	{

	}

	void ConfigurationManager::setConfigurationFilePath(const File &file)
	{
		_saveFile = file;
	}

	bool ConfigurationManager::saveToFile()
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

	bool ConfigurationManager::loadFile()
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

}