#pragma once

#include <Utils/Dependency.hpp>
#include <Utils/File.hpp>
#include <cereal/external/rapidjson/rapidjson.h>
#include <cereal/external/rapidjson/document.h>
#include <iostream>
#include <Core/Engine.hh>
#include <vector>
#include <set>
#include <MediaFiles/AssetsManager.hpp>
#include <Sprite/Sprite.hh>

class SpriteManager : public Dependency<SpriteManager>
{
public:
	SpriteManager()
//		: _vertexManager(nullptr)
	{}

	virtual ~SpriteManager()
	{}

	bool init()
	{
		//std::array<Attribute, 2> param =
		//{
		//	Attribute(GL_FLOAT, sizeof(float), 4),
		//	Attribute(GL_FLOAT, sizeof(float), 2),
		//};
		//_vertexManager = std::make_unique<VertexManager<2>>(param);
		//return _vertexManager->init();
		return true;
	}

	bool loadFile(const File &file)
	{
		//if (!file.exists())
		//	return false;

		//auto content = file.getFileContent();

		//rapidjson::Document document;
		//document.Parse<0>(content.c_str());

		//if (!document.IsObject())
		//{
		//	std::cerr << "Document is not an object." << std::endl;
		//	return false;
		//}

		//if (!document.HasMember("name") || !document["name"].IsString())
		//{
		//	std::cerr << "Document don't have member [name]." << std::endl;
		//	return false;
		//}


		//if (!document.HasMember("image") || !document["image"].IsString())
		//{
		//	std::cerr << "Document don't have member [image]." << std::endl;
		//	return false;
		//}

		//std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>();

		//std::string image = document["image"].GetString();
		//auto texture = _dependencyManager.lock()->getInstance<AssetsManager>()->loadFromFile(file.getFolder() + "/" + image);

		//if (!texture.get())
		//{
		//	std::cerr << "Texture [" << image << "] not found.";
		//	return false;
		//}

		//sprite->_name = document["name"].GetString();

		//if (!document.HasMember("frames") || !document["frames"].IsObject())
		//{
		//	std::cerr << "Document don't have member [frames]." << std::endl;
		//	return false;
		//}

		//if (!document.HasMember("animations") || !document["animations"].IsObject())
		//{
		//	std::cerr << "Document don't have member [frames]." << std::endl;
		//	return false;
		//}

		//std::map<std::string, std::shared_ptr<SpriteFrame>> frames;

		//for (rapidjson::Value::ConstMemberIterator itr = document["frames"].MemberBegin(); itr != document["frames"].MemberEnd(); ++itr)
		//{
		//	if (!itr->value.HasMember("uvs") || !itr->value.HasMember("dimensions") || !itr->value.HasMember("alias"))
		//	{
		//		std::cerr << "Frame [" << itr->name.GetString() << "] is not valid." << std::endl;
		//		return false;
		//	}

		//	std::shared_ptr<SpriteFrame> f = std::make_shared<SpriteFrame>();

		//	// LOAD UVS
		//	const rapidjson::Value& uvs = itr->value["uvs"];
		//	if (!uvs.IsArray() || uvs.Size() != 4)
		//	{
		//		std::cerr << "uvs is not valid." << std::endl;
		//		return false;
		//	}

		//	for (rapidjson::SizeType i = 0; i < uvs.Size(); i++)
		//	{
		//		f->_uvs[i] = static_cast<float>(uvs[i].GetDouble());
		//	}

		//	// LOAD DIMENSIONS
		//	const rapidjson::Value& dimensions = itr->value["dimensions"];
		//	if (!dimensions.IsArray() || dimensions.Size() != 4)
		//	{
		//		std::cerr << "dimensions is not valid." << std::endl;
		//		return false;
		//	}

		//	for (rapidjson::SizeType i = 0; i < dimensions.Size(); i++)
		//	{
		//		f->_dimensions[i] = dimensions[i].GetUint();
		//	}
		//}
		return true;
	}

	std::shared_ptr<SpriteAnimation> getAnimation(const std::string &spriteName, const std::string &animationName) const
	{
		if (_collection.find(spriteName) == std::end(_collection))
			return nullptr;
		return _collection.at(spriteName)->getAnimation(animationName);
	}

private:
//	std::unique_ptr<VertexManager<2>> _vertexManager;
	std::map<std::string, std::shared_ptr<Sprite>> _collection;
};