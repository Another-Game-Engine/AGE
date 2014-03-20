#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "System.h"
#include <Components/SpriteComponent.hh>
#include <Entities/EntityData.hh>
#include <Context/IRenderContext.hh>
#include <Components/CameraComponent.hpp>

class SpriteSystem : public System
{
public:
	SpriteSystem(std::weak_ptr<AScene> scene)
		: System(scene)
		, _filter(scene)
		, _cameras(scene)
	{
		_name = "sprite_system";
	}
	virtual ~SpriteSystem(){}
private:
	EntityFilter _filter;
	EntityFilter _cameras;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		std::shared_ptr<Component::Sprite> sprite;
		for (auto e : _filter.getCollection())
		{
			sprite = e->getComponent<Component::Sprite>();
			sprite->update(static_cast<float>(time));
		}
	}

	virtual void initialize()
	{
		_filter.requireComponent<Component::Sprite>();
	}

};