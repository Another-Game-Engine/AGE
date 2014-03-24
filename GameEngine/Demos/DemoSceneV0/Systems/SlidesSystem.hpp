#pragma once

#include <Systems/System.h>
#include <Entities/EntityData.hh>
#include <Components/HotZone.hpp>
#include <Components/Collision.hpp>
#include <MyTags.hpp>
#include <Rooms/Room.hpp>
#include <Context/SdlContext.hh>

class SlidesSystem : public System
{
public:
	SlidesSystem(std::weak_ptr<AScene> scene)
		: System(scene)
		, _slide(scene)
	{}

	virtual ~SlidesSystem()
	{}

private:
	EntityFilter _slide;
	std::shared_ptr<Input> _inputs;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		float ftime = (float)(time);
		static float delay = 0.0f;

		auto scene = _scene.lock();
		static std::size_t counter = 1;
		std::size_t max = 7;
		if (_inputs->getInput(SDL_BUTTON_RIGHT) && _slide.getCollection().size() > 0 && delay <= 0.0f)
		{
			auto s = (*(_slide.getCollection().begin())).get()->getComponent<Component::Sprite>();
			if (counter >= max)
				counter = 0;
			s->animation = scene->getInstance<SpriteManager>()->getAnimation("Slides", "slide" + std::to_string(++counter));
			delay = 0.5f;
		}
		if (delay >= 0.0f)
			delay -= ftime;
	}

	virtual bool initialize()
	{
		_slide.requireComponent<Component::Sprite>();
		_slide.requireTag(MyTags::SLIDE_TAG);
		_inputs = _scene.lock()->getInstance<Input>();
		if (!_inputs)
			return false;
		return true;
	}
};