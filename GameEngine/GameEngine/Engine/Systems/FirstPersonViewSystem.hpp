#ifndef   __FPVIEW_SYS_SYSTEM_HPP__
# define  __FPVIEW_SYS_SYSTEM_HPP__

#include <Utils/MatrixConversion.hpp>
#include "System.h"
#include <Components/CameraComponent.hpp>
#include <Components/FirstPersonView.hpp>
#include <Core/Engine.hh>


class FirstPersonViewSystem : public System
{
public:
	FirstPersonViewSystem(std::weak_ptr<AScene> &&scene)
		: System(std::move(scene))
		, _filter(std::move(scene))
	{
		_name = "first_person_view_system";
	}

	virtual ~FirstPersonViewSystem(){}
protected:
	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		auto scene = _scene.lock();
		for (auto e : _filter.getCollection())
		{
			auto lookAt = scene->getTransform(e);
			auto camera = scene->getComponent<Component::CameraComponent>(e);
			lookAt = glm::translate(lookAt, glm::vec3(0, 0, 1));
			camera->lookAtTransform = glm::lookAt(posFromMat4(scene->getTransform(e)), posFromMat4(lookAt), glm::vec3(0, 1, 0));
		}
	}

	virtual bool initialize()
	{
		_filter.requireComponent<Component::CameraComponent>();
		_filter.requireComponent<Component::FirstPersonView>();
		return true;
	}
protected:
	EntityFilter _filter;
};

#endif    //__FPVIEW_SYS_SYSTEM_HPP__