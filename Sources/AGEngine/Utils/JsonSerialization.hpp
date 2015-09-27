#pragma once

#include <cereal/external/rapidjson/rapidjson.h>
#include <cereal/external/rapidjson/document.h>

namespace JsonSerialization
{
	template <typename T>
	void save(const T &value, rapidjson::Value &json, rapidjson::Document &document);

	template <typename T>
	void load(T &value, rapidjson::Value &json, rapidjson::Document &document);
}