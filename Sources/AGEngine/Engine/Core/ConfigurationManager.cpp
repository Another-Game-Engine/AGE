#include "ConfigurationManager.hpp"
#include <fstream>

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

	ConfigurationManager::ConfigurationManager(const OldFile &saveFile /*= File("./ConfigurationSaveFile.conf")*/)
		: _saveFile(saveFile)
	{
	}

	ConfigurationManager::~ConfigurationManager()
	{

	}

	void ConfigurationManager::setConfigurationFilePath(const OldFile &file)
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
		{
			saveToFile();
			return false;
		}
		auto content = _saveFile.getFileContent();

		rapidjson::Document document;
		document.Parse<0>(content.c_str());

		if (!document.IsObject())
			return false;

		for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin();
			itr != document.MemberEnd(); ++itr)
		{
			auto name = itr->name.GetString();
			if (_confs.find(name) != std::end(_confs))
			{
				_confs[name]->load(document[name], document);
			}
			else
			{
				auto member = document[name].MemberBegin();
				if (member->value.IsString())
				{
					setConfiguration<std::string>(std::string(name), "");
					_confs[name]->load(document[name], document);
				}
				else if (member->value.IsInt())
				{
					setConfiguration<int>(std::string(name), 0);
					_confs[name]->load(document[name], document);
				}
				else if (member->value.IsUint())
				{
					setConfiguration<unsigned int>(std::string(name), 0);
					_confs[name]->load(document[name], document);
				}
				else if (member->value.IsBool_())
				{
					setConfiguration<bool>(std::string(name), false);
					_confs[name]->load(document[name], document);
				}
			}
		}
		return true;
	}

}