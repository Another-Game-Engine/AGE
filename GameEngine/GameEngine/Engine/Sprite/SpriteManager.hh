#pragma once

#include <Utils/Dependency.hpp>
#include <Utils/File.hpp>
#include <cereal/external/rapidjson/rapidjson.h>
#include <cereal/external/rapidjson/document.h>
#include <iostream>
#include <Core/Engine.hh>
#include <vector>
#include <set>

class SpriteManager : public Dependency
{
private:
	struct LoadFrame
	{
		float uvs[4];
		unsigned int dimensions[2];
	};

	struct LoadAnimation
	{
		std::vector<unsigned int> steps;
		std::vector<std::shared_ptr<LoadFrame>> frames;
	};

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

		std::string image = document["image"].GetString();

		if (!document.HasMember("frames") || !document["frames"].IsObject())
		{
			std::cerr << "Document don't have member [frames]." << std::endl;
			return false;
		}

		if (!document.HasMember("animations") || !document["animations"].IsObject())
		{
			std::cerr << "Document don't have member [frames]." << std::endl;
			return false;
		}

		std::map<std::string, std::shared_ptr<LoadFrame>> frames;

		for (rapidjson::Value::ConstMemberIterator itr = document["frames"].MemberBegin(); itr != document["frames"].MemberEnd(); ++itr)
		{
			if (!itr->value.HasMember("uvs") || !itr->value.HasMember("dimensions") || !itr->value.HasMember("alias"))
			{
				std::cerr << "Frame [" << itr->name.GetString() << "] is not valid." << std::endl;
				return false;
			}

			std::shared_ptr<LoadFrame> f = std::make_shared<LoadFrame>();

			// LOAD UVS
			const rapidjson::Value& uvs = itr->value["uvs"];
			if (!uvs.IsArray() || uvs.Size() != 4)
			{
				std::cerr << "uvs is not valid." << std::endl;
				return false;
			}

			for (rapidjson::SizeType i = 0; i < uvs.Size(); i++)
			{
				f->uvs[i] = uvs[i].GetDouble();
			}

			// LOAD DIMENSIONS
			const rapidjson::Value& dimensions = itr->value["dimensions"];
			if (!dimensions.IsArray() || dimensions.Size() != 2)
			{
				std::cerr << "dimensions is not valid." << std::endl;
				return false;
			}

			for (rapidjson::SizeType i = 0; i < dimensions.Size(); i++)
			{
				f->dimensions[i] = dimensions[i].GetUint();
			}

			// LOAD ALIAS
			const rapidjson::Value& alias = itr->value["alias"];
			if (!alias.IsArray())
			{
				std::cerr << "alias is not valid." << std::endl;
				return false;
			}

			for (rapidjson::SizeType i = 0; i < alias.Size(); i++)
			{
				frames.insert(std::make_pair(alias[i].GetString(), f));
			}
			frames.insert(std::make_pair(itr->name.GetString(), f));
		}

		// LOAD ANIMATIONS
		std::map<std::string, std::shared_ptr<LoadAnimation>> animations;

		for (rapidjson::Value::ConstMemberIterator itr = document["animations"].MemberBegin(); itr != document["animations"].MemberEnd(); ++itr)
		{
			auto animation = std::make_shared<LoadAnimation>();
			if (!itr->value.IsArray())
			{
				std::cerr << "Animation [" << itr->name.GetString() << "] is not valid." << std::endl;
				return false;
			}

			// LOAD ANIMATION
			std::map<std::shared_ptr<LoadFrame>, unsigned int> tmpRef;
			for (rapidjson::SizeType i = 0; i < itr->value.Size(); i++)
			{
				auto name = itr->value[i].GetString();
				auto address = frames.find(name);
				auto ref = tmpRef.find(address->second);
				if (ref == std::end(tmpRef))
				{
					tmpRef.insert(std::make_pair(address->second, animation->frames.size()));
					ref = tmpRef.find(address->second);
					animation->frames.push_back(address->second);
				}
				animation->steps.push_back(ref->second);
			}
			animations.insert(std::make_pair(itr->name.GetString(), animation));
		}
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