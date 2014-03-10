#pragma once

#include <Utils/Dependency.hpp>
#include <Utils/File.hpp>
#include <cereal/external/rapidjson/rapidjson.h>
#include <cereal/external/rapidjson/document.h>
#include <iostream>
#include <Core/Engine.hh>

class SpriteManager : public Dependency
{
public:
	SpriteManager()
	{}

	virtual ~SpriteManager()
	{}

	bool init(Engine *e)
	{
		_engine = e;
		return true;
	}

	bool loadFile(const File &file)
	{
		if (!file.exists())
			return false;

		auto content = file.getFileContent();

		rapidjson::Document document;
		document.Parse<0>(content.c_str());

		if (!document.IsObject())
		{
			std::cerr << "Document is not an object." << std::endl;
			return false;
		}

		if (!document.HasMember("image") || !document["image"].IsString())
		{
			std::cerr << "Document don't have member [image]." << std::endl;
			return false;
		}

		std::cout << "image = " << document["image"].GetString() << std::endl;

		//assert(document["t"].IsBool());         // JSON true/false are bool. Can also uses more specific function IsTrue().
		//printf("t = %s\n", document["t"].GetBool() ? "true" : "false");

		//assert(document["f"].IsBool());
		//printf("f = %s\n", document["f"].GetBool() ? "true" : "false");

		//printf("n = %s\n", document["n"].IsNull() ? "null" : "?");

		//assert(document["i"].IsNumber());       // Number is a JSON type, but C++ needs more specific type.
		//assert(document["i"].IsInt());          // In this case, IsUint()/IsInt64()/IsUInt64() also return true.
		//printf("i = %d\n", document["i"].GetInt());     // Alternative (int)document["i"]

		//assert(document["pi"].IsNumber());
		//assert(document["pi"].IsDouble());
		//printf("pi = %g\n", document["pi"].GetDouble());

		//const Value& a = document["a"]; // Using a reference for consecutive access is handy and faster.
		//assert(a.IsArray());
		//for (SizeType i = 0; i < a.Size(); i++) // rapidjson uses SizeType instead of size_t.
		//	printf("a[%d] = %d\n", i, a[i].GetInt());
		return true;
	}
private:
	Engine *_engine;
};