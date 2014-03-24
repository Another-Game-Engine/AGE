#pragma once

#include <Systems/System.h>
#include <Entities/EntityData.hh>
#include <Components/Collision.hpp>
#include <Context/SdlContext.hh>
#include <Systems/CameraSystem.hpp>
#include <Components/RigidBody.hpp>
#include <MyTags.hpp>

class FPControllerSystem;

class SceneInSceneSystem : public System
{
public:
	SceneInSceneSystem(std::weak_ptr<AScene> scene)
		: System(scene)
	{}

	virtual ~SceneInSceneSystem()
	{}

	std::string sceneName = "";
	bool isOn = false;
	bool watch = false;

	void setScene(const std::string &_sceneName)
	{
		if (_sceneName.size() == 0)
		{
			sceneName = "";
			isOn = false;
			watch = false;
		}
		else if (sceneName.length() == 0)
		{
			sceneName = _sceneName;
			isOn = false;
			watch = true;
		}
	}

private:
	std::shared_ptr<Input> _inputs;
	std::shared_ptr<FontManager> _fontManager;
	
	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		if (!watch)
			return;

		float ftime = (float)(time);
		static float delay = 0.0f;
		auto scene = _scene.lock();

		if (isOn && _inputs->getKey(SDLK_q) && delay <= 0.0f)
		{
			PubSub t(_scene.lock()->getDependenciesInjectorParent().lock()->getInstance<PubSub::Manager>());
			t.broadCast(PubSubKey(sceneName + "Pause"));
			isOn = false;
			scene->activateSystem<FPControllerSystem>();
			delay = 0.1f;
		}
		else if (!isOn && _inputs->getKey(SDLK_e) && delay <= 0.0f)
		{
			PubSub t(_scene.lock()->getDependenciesInjectorParent().lock()->getInstance<PubSub::Manager>());
			t.broadCast(PubSubKey(sceneName + "Play"));
			isOn = true;
			scene->deactivateSystem<FPControllerSystem>();
			delay = 0.1f;
		}
		if (delay >= 0.0f)
			delay -= ftime;
		if (isOn)
		{
			_fontManager->draw2DString("Press Q to exit current Scene.", "myFont", 50, glm::ivec2(10, 900), glm::vec4(0.0f, 0.59607f, 0.941f, 1.0f), "2DText");
		}
		else
		{
			_fontManager->draw2DString("Press E to join current Scene.", "myFont", 50, glm::ivec2(10, 900), glm::vec4(0.0f, 0.59607f, 0.941f, 1.0f), "2DText");
		}
	}

	virtual bool initialize()
	{
		_inputs = _scene.lock()->getInstance<Input>();
		_fontManager = _scene.lock()->getInstance<FontManager>();

		if (!_inputs)
			return false;
		return true;
	}

	virtual bool activate()
	{
		return true;
	}

	virtual bool deactivate()
	{
		return true;
	}
};