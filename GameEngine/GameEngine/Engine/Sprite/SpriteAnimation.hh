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

	void draw(std::uint32_t index) const
	{
		_frames[_steps[static_cast<std::size_t>(index)]]->draw();
	}

	inline std::uint32_t getStepNumber() const { return _steps.size(); }
private:
	std::vector<std::uint32_t> _steps;
	std::vector<std::shared_ptr<SpriteFrame>> _frames;
	std::shared_ptr<TextureFile> _texture;
	std::string _name;
	friend class SpriteManager;
};