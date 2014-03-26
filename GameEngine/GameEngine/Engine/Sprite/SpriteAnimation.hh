#pragma once

#include <Sprite/SpriteFrame.hh>
#include <memory>
#include <MediaFiles/TextureFile.hpp>
#include <MediaFiles/MaterialFile.hpp>

class SpriteAnimation
{
public:
	SpriteAnimation(){}
	~SpriteAnimation(){}

	inline const std::string &getName() const { return _name; }
	inline MaterialFile::Material &getMaterial() { return _material; }

	void draw(std::uint32_t index) const
	{
		_frames[_steps[static_cast<std::size_t>(index)]]->draw();
	}

	inline std::uint32_t getStepNumber() const { return _steps.size(); }
//private:
	bool _alphaTest = true;
	std::vector<std::uint32_t> _steps;
	std::vector<std::shared_ptr<SpriteFrame>> _frames;
	//	std::shared_ptr<TextureFile> _texture;
	MaterialFile::Material _material;
	std::string _name;
	friend class SpriteManager;
};