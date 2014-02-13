#ifndef   __TRACKBALL_SYSTEM_HPP__
# define  __TRACKBALL_SYSTEM_HPP__

#include <Utils/MatrixConversion.hpp>
#include "System.h"
#include <Components/CameraComponent.hpp>
#include <Components/TrackBallComponent.hpp>
#include <Entities/EntityData.hh>
#include <Core/Input.hh>
#include <Core/Engine.hh>
#include <Context/SdlContext.hh>


class TrackBallSystem : public System
{
public:
	TrackBallSystem(AScene *scene)
		: System(scene)
		, _filter(scene)
	{}

	virtual ~TrackBallSystem(){}
protected:

	EntityFilter _filter;

	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		auto inputs = _scene->getInstance<Input>();

		for (auto e : _filter.getCollection())
		{
			auto camera = e->getComponent<Component::CameraComponent>();
			auto trackBall = e->getComponent<Component::TrackBall>();

			glm::vec3		pos;

			trackBall->dist -= inputs->getMouseWheel().y * trackBall->zoomSpeed;
			trackBall->angles -= glm::vec2((float)inputs->getMouseDelta().x / float(1000 / trackBall->rotateSpeed), -(float)inputs->getMouseDelta().y / float(1000 / trackBall->rotateSpeed));
			if (abs(trackBall->dist) < 0.0001f)
			{
				if (trackBall->dist < 0)
					trackBall->dist = -0.0001f;
				else
					trackBall->dist = 0.0001f;
			}
			pos.x = sin(trackBall->angles.x) * cos(trackBall->angles.y) * trackBall->dist;
			pos.y = sin(trackBall->angles.y) * trackBall->dist;
			pos.z = cos(trackBall->angles.x) * cos(trackBall->angles.y) * trackBall->dist;
			e->setLocalTransform() = glm::lookAt(glm::vec3(trackBall->toLook->getGlobalTransform()[3]) + pos,
				glm::vec3(trackBall->toLook->getGlobalTransform()[3]),
				glm::vec3(0, 1, 0));
			camera->setLookAtTransform() = e->getLocalTransform();
		}
	}

	virtual void initialize()
	{
		_filter.requireComponent<Component::CameraComponent>();
		_filter.requireComponent<Component::TrackBall>();
		SDL_SetRelativeMouseMode(SDL_bool(true));
	}
};

#endif    //__TRACKBALL_SYSTEM_HPP__