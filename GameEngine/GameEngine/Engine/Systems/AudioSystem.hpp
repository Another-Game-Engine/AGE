#pragma once

#include <Systems/System.h>
#include <Entities/EntityData.hh>
#include <Components/AudioListener.hpp>
#include <Audio/AudioManager.hpp>
#include <Core/Engine.hh>
#include <Utils/MatrixConversion.hpp>

class AudioSystem : public System
{
public:
	AudioSystem(AScene *scene)
		: System(scene)
		, _emitters(scene)
		, _listeners(scene)
		, _manager(&scene->getEngine().getInstance<AudioManager>())
	{
		assert(_manager != nullptr && "No audio manager found.");
	}
	virtual ~AudioSystem(){}
protected:
	EntityFilter _emitters;
	EntityFilter _listeners;
	AudioManager *_manager;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		_manager->update();

		for (auto e : _listeners.getCollection())
		{
			auto l = e->getComponent<Component::AudioListener>();
			auto pos = posFromMat4(e->getGlobalTransform());
			FMOD_VECTOR v{pos.x, pos.y, pos.z};
			FMOD_VECTOR up{ 0, -1, 0 };
			_manager->getSystem()->set3DListenerAttributes(0, &v, 0, 0, &up);
		}
	}

	virtual void initialize()
	{
		_listeners.require<Component::AudioListener>();
	}
};