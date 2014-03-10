#pragma once

#include <Sprite/SpriteFrame.hh>
#include <memory>
#include <MediaFiles/TextureFile.hpp>

class SpriteAnimation
{
public:
	SpriteAnimation(){}
	~SpriteAnimation(){}

	inline const std::string &getName() const { return _name; }
	inline std::shared_ptr<TextureFile> getTexture() const { return _texture; }
private:
	std::vector<unsigned int> _steps;
	std::vector<std::shared_ptr<SpriteFrame>> _frames;
	std::shared_ptr<TextureFile> _texture;
	std::string _name;
	friend class SpriteManager;
};