#pragma once

#include <Sprite/SpriteAnimation.hh>

class Sprite
{
public:
	Sprite(){}
	~Sprite(){}
	std::shared_ptr<SpriteAnimation> getAnimation(const std::string &name) const
	{
		auto r = _animations.find(name);
		if (r == std::end(_animations))
			return nullptr;
		return r->second;
	}
	inline const std::string &getName() const { return _name; }
private:
	std::map<std::string, std::shared_ptr<SpriteAnimation>> _animations;
	std::string _name;
	friend class SpriteManager;
};