#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/SpriteComponent.hh>

class SpriteSystem : public System
{
public:
	SpriteSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _filter(std::move(scene))
	{
		_name = "sprite_system";
	}
	virtual ~SpriteSystem(){}
private:
	EntityFilter _filter;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		auto scene = _scene.lock();
		Component::Sprite *sprite = nullptr;
		for (auto e : _filter.getCollection())
		{
			sprite = scene->getComponent<Component::Sprite>(e);
			sprite->update(static_cast<float>(time));
		}
	}

	virtual bool initialize()
	{
		_filter.requireComponent<Component::Sprite>();
		return true;
	}

};