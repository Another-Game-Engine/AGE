#pragma once

#include <Systems/System.h>
#include <Entities/EntityData.hh>
#include <Components/AudioListener.hpp>
#include <Components/AudioEmitter.hpp>
#include <Audio/AudioManager.hh>
#include <Core/Engine.hh>
#include <Utils/MatrixConversion.hpp>
#include <Components/Collision.hpp>

class BallSoundSystem : public System
{
public:
	BallSoundSystem(AScene *scene)
		: System(scene)
		, _balls(scene)
	{
	}
	virtual ~BallSoundSystem(){}
protected:
	EntityFilter _balls;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		for (auto e : _balls.getCollection())
		{
			auto ae = e->getComponent<Component::AudioEmitter>();
			auto force = e->getComponent<Component::Collision>()->force;
//			std::cout << force << std::endl;
			if (force > 1.0f)
				ae->play("collision", false);
		}
	}

	virtual void initialize()
	{
		_balls.require<Component::AudioEmitter>();
		_balls.require<Component::Collision>();
	}
};