#include "JsonSerialization.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace JsonSerialization
{

	void save(const int &value, rapidjson::Value &json, rapidjson::Document &document)
	{
		rapidjson::Value v(rapidjson::kNumberType);
		v.SetInt(value);
		json.AddMember("int", v, document.GetAllocator());
	}

	void load(int &value, rapidjson::Value &json, rapidjson::Document &document)
	{
		if (!json.HasMember("int"))
			return;
		value = json["int"].GetInt();
	}

	void save(const unsigned int &value, rapidjson::Value &json, rapidjson::Document &document)
	{
		rapidjson::Value v(rapidjson::kNumberType);
		v.SetUint(value);
		json.AddMember("unsigned int", v, document.GetAllocator());
	}

	void load(unsigned int &value, rapidjson::Value &json, rapidjson::Document &document)
	{
		if (!json.HasMember("unsigned int"))
			return;
		value = json["unsigned int"].GetUint();
	}

	void save(const bool &value, rapidjson::Value &json, rapidjson::Document &document)
	{
		rapidjson::Value v;
		v.SetBool_(value);
		json.AddMember("bool", v, document.GetAllocator());
	}

	void load(bool &value, rapidjson::Value &json, rapidjson::Document &document)
	{
		if (!json.HasMember("bool"))
			return;
		value = json["bool"].GetBool_();
	}

	void save(const glm::uvec2 &value, rapidjson::Value &json, rapidjson::Document &document)
	{
		rapidjson::Value v(rapidjson::kArrayType);
		v.PushBack(value.x, document.GetAllocator()).PushBack(value.y, document.GetAllocator());
		json.AddMember("uvec2", v, document.GetAllocator());
	}

	void load(glm::uvec2 &value, rapidjson::Value &json, rapidjson::Document &document)
	{
		if (!json.HasMember("uvec2"))
			return;
		value.x = json["uvec2"][(rapidjson::SizeType)(0)].GetUint();
		value.y = json["uvec2"][(rapidjson::SizeType)(1)].GetUint();
	}

	void save(const std::string &value, rapidjson::Value &json, rapidjson::Document &document)
	{
		rapidjson::Value v(rapidjson::kArrayType);
		json.AddMember("string", value.c_str(), document.GetAllocator());
	}

	void load(std::string &value, rapidjson::Value &json, rapidjson::Document &document)
	{
		if (!json.HasMember("string"))
			return;
		value = json["string"].GetString();
	}

}