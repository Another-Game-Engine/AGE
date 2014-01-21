#ifndef   __SPACESHIP_CONTROLLER_HPP__
# define  __SPACESHIP_CONTROLLER_HPP__

#include <Components/Component.hh>
#include "Core/Engine.hh"
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "Context/SdlContext.hh"
#include <Managers/BulletDynamicManager.hpp>
#include <array>

namespace Component
{
	ATTRIBUTE_ALIGNED16(struct) SpaceshipController : public ComponentBase<SpaceshipController>
	{
		enum CONTROLS
		{
			FORWARD = 0,
			BACKWARD,
			LEFT,
			RIGHT,
			SHOOT
		};

		BT_DECLARE_ALIGNED_ALLOCATOR();
		SpaceshipController()
			: ComponentBase<SpaceshipController>()
		{
			setKey(LEFT, SDLK_a);
			setKey(RIGHT, SDLK_d);
			setKey(FORWARD, SDLK_w);
			setKey(BACKWARD, SDLK_s);
			setKey(SHOOT, SDLK_SPACE);
			controls.fill(false);
		}

		virtual ~SpaceshipController()
		{}

		void init()
		{}

		virtual void reset()
		{}

		void setKey(CONTROLS k, unsigned int key)
		{
			if (k >= 5)
				return;
			keys[k] = key;
		}

		void resetControls()
		{
			controls.fill(false);
		}

		std::array<unsigned int, 5> keys;
		std::array<bool, 5> controls;
	};
}

#endif    //__SPACESHIP_CONTROLLER_HPP__