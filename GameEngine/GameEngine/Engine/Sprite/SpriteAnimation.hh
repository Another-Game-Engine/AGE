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

	void update(std::size_t &index) const
	{
		index = ++index % _steps.size();
	}

	void draw(std::size_t index) const
	{
		_frames[_steps[index]]->draw();
	}
private:
	std::vector<unsigned int> _steps;
	std::vector<std::shared_ptr<SpriteFrame>> _frames;
	std::shared_ptr<TextureFile> _texture;
	std::string _name;
	friend class SpriteManager;
};