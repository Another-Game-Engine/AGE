#pragma once

#include <glm/fwd.hpp>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <Systems/System.h>
#include <Components/FPController.hpp>
//#include <Physic/BulletDynamicManager.hpp>
//#include <Core/Engine.hh>
//#include <Context/SdlContext.hh>
//#include <Utils/MatrixConversion.hpp>


namespace AGE
{
	class FPControllerSystem : public System
	{
	public:
		FPControllerSystem(std::weak_ptr<AScene> &&scene);
		virtual ~FPControllerSystem();
	private:
		std::shared_ptr<BulletCollisionManager> _manager;
		EntityFilter _filter;

		virtual void updateBegin(double time);
		virtual void updateEnd(double time);
		virtual void mainUpdate(double time);
		void updateComponent(Entity &entity, Component::FPController *fp, double time);
		virtual bool initialize();
	};
}