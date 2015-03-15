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
		FPControllerSystem(AScene *scene);
		virtual ~FPControllerSystem();
	private:
		std::shared_ptr<BulletCollisionManager> _manager;
		EntityFilter _filter;

		virtual void updateBegin(float time);
		virtual void updateEnd(float time);
		virtual void mainUpdate(float time);
		void updateComponent(Entity &entity, FPController *fp, float time);
		virtual bool initialize();
	};
}