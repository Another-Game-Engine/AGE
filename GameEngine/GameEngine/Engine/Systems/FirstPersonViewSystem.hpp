#ifndef   __FPVIEW_SYS_SYSTEM_HPP__
# define  __FPVIEW_SYS_SYSTEM_HPP__

#include <Utils/MatrixConversion.hpp>
#include "System.h"
#include <Components/CameraComponent.hh>
#include <Components/FirstPersonView.hpp>
#include <Entities/Entity.hh>
#include <Core/Engine.hh>


class FirstPersonViewSystem : public System
{
public:
	FirstPersonViewSystem(Engine &engine)
		: System(engine)
	{}
	virtual ~FirstPersonViewSystem(){}
protected:
	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		for (auto e : _collection)
		{
			auto lookAt = e->getGlobalTransform();
			auto camera = e->getComponent<Component::CameraComponent>();
			lookAt = glm::translate(lookAt, glm::vec3(0, 0, 1));
			camera->setLookAtTransform() = glm::lookAt(posFromMat4(e->getGlobalTransform()), posFromMat4(lookAt), glm::vec3(0, 1, 0));
		}
	}

	virtual void initialize()
	{
		require<Component::CameraComponent>();
		require<Component::FirstPersonView>();
	}
};

#endif    //__FPVIEW_SYS_SYSTEM_HPP__