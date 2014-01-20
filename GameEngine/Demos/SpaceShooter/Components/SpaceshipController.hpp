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
	ATTRIBUTE_ALIGNED16(class) SpaceshipController : public ComponentBase<SpaceshipController>
	{
	public:
		enum CONTROLS
		{
			FORWARD = 0,
			BACKWARD,
			LEFT,
			RIGHT,
			SHOOT
		};
	public:
		BT_DECLARE_ALIGNED_ALLOCATOR();
		SpaceshipController()
			: ComponentBase<SpaceshipController>()
		{
			setKey(LEFT, SDLK_a);
			setKey(RIGHT, SDLK_d);
			setKey(FORWARD, SDLK_w);
			setKey(BACKWARD, SDLK_s);
			setKey(SHOOT, SDLK_SPACE);
			_controls.fill(false);
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
			_keys[k] = key;
		}

		std::array<unsigned int, 5> &getKeys()
		{
			return _keys;
		}

		std::array<bool, 5> &getControls()
		{
			return _controls;
		}

		void resetControls()
		{
			_controls.fill(false);
		}
	protected:
		std::array<unsigned int, 5> _keys;
		std::array<bool, 5> _controls;
	};
}

#endif    //__SPACESHIP_CONTROLLER_HPP__