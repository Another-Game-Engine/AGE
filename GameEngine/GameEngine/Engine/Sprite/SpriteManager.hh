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

class SpriteManager : public Dependency
{
public:
	SpriteManager()
		: _engine(nullptr)
		, _vertexManager(nullptr)
	{}

	virtual ~SpriteManager()
	{}

	bool init(Engine *e)
	{
		_engine = e;
		std::array<Attribute, 2> param =
		{
			Attribute(GL_FLOAT, sizeof(float), 4),
			Attribute(GL_FLOAT, sizeof(float), 2),
		};
		_vertexManager = std::make_unique<VertexManager<2>>(param);
		return _vertexManager->init();
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

		if (!document.HasMember("name") || !document["name"].IsString())
		{
			std::cerr << "Document don't have member [name]." << std::endl;
			return false;
		}


		if (!document.HasMember("image") || !document["image"].IsString())
		{
			std::cerr << "Document don't have member [image]." << std::endl;
			return false;
		}

		std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>();

		std::string image = document["image"].GetString();
		auto texture = _engine->getInstance<AssetsManager>()->loadFromFile<cereal::BinaryInputArchive>(file.getFolder() + "/" + image);

		if (!texture.get())
		{
			std::cerr << "Texture [" << image << "] not found.";
			return false;
		}

		sprite->_name = document["name"].GetString();

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

		std::map<std::string, std::shared_ptr<SpriteFrame>> frames;

		for (rapidjson::Value::ConstMemberIterator itr = document["frames"].MemberBegin(); itr != document["frames"].MemberEnd(); ++itr)
		{
			if (!itr->value.HasMember("uvs") || !itr->value.HasMember("dimensions") || !itr->value.HasMember("alias"))
			{
				std::cerr << "Frame [" << itr->name.GetString() << "] is not valid." << std::endl;
				return false;
			}

			std::shared_ptr<SpriteFrame> f = std::make_shared<SpriteFrame>();

			// LOAD UVS
			const rapidjson::Value& uvs = itr->value["uvs"];
			if (!uvs.IsArray() || uvs.Size() != 4)
			{
				std::cerr << "uvs is not valid." << std::endl;
				return false;
			}

			for (rapidjson::SizeType i = 0; i < uvs.Size(); i++)
			{
				f->_uvs[i] = uvs[i].GetDouble();
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
				f->_dimensions[i] = dimensions[i].GetUint();
			}

			// LOAD FRAME
			if (!f->load(_vertexManager))
			{
				std::cerr << "Frame failed to load." << std::endl;
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

		for (rapidjson::Value::ConstMemberIterator itr = document["animations"].MemberBegin(); itr != document["animations"].MemberEnd(); ++itr)
		{
			auto animation = std::make_shared<SpriteAnimation>();
			if (!itr->value.IsArray())
			{
				std::cerr << "Animation [" << itr->name.GetString() << "] is not valid." << std::endl;
				return false;
			}

			// LOAD ANIMATION
			std::map<std::shared_ptr<SpriteFrame>, unsigned int> tmpRef;
			for (rapidjson::SizeType i = 0; i < itr->value.Size(); i++)
			{
				auto name = itr->value[i].GetString();
				auto address = frames.find(name);
				auto ref = tmpRef.find(address->second);
				if (ref == std::end(tmpRef))
				{
					tmpRef.insert(std::make_pair(address->second, animation->_frames.size()));
					ref = tmpRef.find(address->second);
					animation->_frames.push_back(address->second);
				}
				animation->_texture = std::static_pointer_cast<TextureFile>(texture);
				animation->_steps.push_back(ref->second);
			}
			sprite->_animations.insert(std::make_pair(itr->name.GetString(), animation));
		}
		_collection.insert(std::make_pair(sprite->_name, sprite));
		return true;
	}

	std::shared_ptr<SpriteAnimation> getAnimation(const std::string &spriteName, const std::string &animationName) const
	{
		if (_collection.find(spriteName) == std::end(_collection))
			return nullptr;
		return _collection.at(spriteName)->getAnimation(animationName);
	}

private:
	Engine *_engine;
	std::unique_ptr<VertexManager<2>> _vertexManager;
	std::map<std::string, std::shared_ptr<Sprite>> _collection;
};