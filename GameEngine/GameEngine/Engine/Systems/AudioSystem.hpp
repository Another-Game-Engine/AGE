#pragma once

#include <Systems/System.h>
#include <Components/AudioListener.hpp>
#include <Components/AudioEmitter.hpp>
#include <Audio/AudioManager.hh>
#include <Core/Engine.hh>
#include <Utils/MatrixConversion.hpp>
#include <glm/glm.hpp>

class AudioSystem : public System
{
public:
	AudioSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _emitters(std::move(scene))
		, _listeners(std::move(scene))
		, _manager(scene.lock()->getInstance<AudioManager>())
	{
		_name = "audio_system";
		assert(_manager != nullptr && "No audio manager found.");
	}
	virtual ~AudioSystem(){}
protected:
	EntityFilter _emitters;
	EntityFilter _listeners;
	std::shared_ptr<AudioManager> _manager;

	virtual void updateBegin(double time)
	{}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		_manager->update();
		auto scene = _scene.lock();

		for (auto e : _emitters.getCollection())
		{
			auto ae = scene->getComponent<Component::AudioEmitter>(e);
			ae->updatePosition();
		}

		for (auto e : _listeners.getCollection())
		{
			auto l = scene->getComponent<Component::AudioListener>(e);
			auto pos = posFromMat4(scene->getGlobalTransform(e));
			glm::mat4 m = scene->getGlobalTransform(e);
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

	virtual bool initialize()
	{
		_listeners.requireComponent<Component::AudioListener>();
		_emitters.requireComponent<Component::AudioEmitter>();
		return true;
	}
};