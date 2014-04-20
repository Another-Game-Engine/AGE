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
}