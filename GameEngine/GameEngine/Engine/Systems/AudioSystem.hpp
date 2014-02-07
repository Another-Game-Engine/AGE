#pragma once

#include <Systems/System.h>
#include <Entities/EntityData.hh>
#include <Components/AudioListener.hpp>
#include <Components/AudioEmitter.hpp>
#include <Audio/AudioManager.hh>
#include <Core/Engine.hh>
#include <Utils/MatrixConversion.hpp>
#include <glm/glm.hpp>

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

		for (auto e : _emitters.getCollection())
		{
			auto ae = e->getComponent<Component::AudioEmitter>();
			ae->updatePosition();
		}

		for (auto e : _listeners.getCollection())
		{
			auto l = e->getComponent<Component::AudioListener>();
			auto pos = posFromMat4(e->getGlobalTransform());
			glm::mat4 m = e->getGlobalTransform();
			glm::vec4 u(0,-1,0,0);
			u = m * u;
			glm::vec4 f(0,0,1,0);
			f = m * f;

			FMOD_VECTOR up{ u.x, u.y, u.z };
			FMOD_VECTOR forward{ f.x, f.y, f.z };
			FMOD_VECTOR v{pos.x, pos.y, pos.z};
			_manager->getSystem()->set3DListenerAttributes(0, &v, 0, &forward, &up);
		}
	}

	virtual void initialize()
	{
		_listeners.require<Component::AudioListener>();
		_emitters.require<Component::AudioEmitter>();
	}
};