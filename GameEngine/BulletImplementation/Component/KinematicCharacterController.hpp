#ifndef   __KINEMATIC_CHARACTER_CONTROLLER_HPP__
# define  __KINEMATIC_CHARACTER_CONTROLLER_HPP__

#include <Components/Component.hh>
#include "Core/Engine.hh"
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include "RigidBody.hpp"
#include "../BulletManager.hpp"

namespace Component
{
	ATTRIBUTE_ALIGNED16(class) KineCharacterController : public ComponentBase<KineCharacterController>
	{
	public:
		BT_DECLARE_ALIGNED_ALLOCATOR();
		KineCharacterController(Engine &engine, Handle &entity)
			: ComponentBase<KineCharacterController>(engine, entity, "KinematicCharacterController")
		{


			btTransform transform;
			glm::vec3 position = posFromMat4(_entity->getLocalTransform());
			glm::vec3 scale = scaleFromMat4(_entity->getLocalTransform());
			std::cout << scale.x << " " << scale.y << " " << scale.z << std::endl;
			glm::vec3 rot = rotFromMat4(_entity->getLocalTransform(), true);
			transform.setIdentity();
			transform.setOrigin(convertGLMVectorToBullet(position));
			transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));

				_ghost = new btPairCachingGhostObject();
				_shape = new btCapsuleShape(1, 1);
				_ghost->setCollisionShape(_shape);
				_ghost->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
				_ghost->setWorldTransform(transform);
				_controller = new btKinematicCharacterController(_ghost, _shape, btScalar(0.35));
				//_controller->setGravity(- _engine.getInstance<BulletManager>().getWorld().getGravity().y());
//				_engine.getInstance<BulletManager>().getWorld().addCharacter(_controller);
				_ghost->setUserPointer(&(_entity));
				_engine.getInstance<BulletManager>().getWorld().addCollisionObject(_ghost, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
				_engine.getInstance<BulletManager>().getWorld().addAction(_controller);
				_engine.getInstance<BulletManager>().getWorld().getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
		}

		virtual ~KineCharacterController()
		{
		}

		void init()
		{}

		virtual void reset()
		{
		}

		void clear()
		{
		}

		btGhostObject &getGhost()
		{
			return *_ghost;
		}

		btKinematicCharacterController &getController()
		{
			return *_controller;
		}

	private:
		btKinematicCharacterController *_controller;
		btPairCachingGhostObject *_ghost;
		btConvexShape *_shape;
		btScalar _stepHeight;
		int _upAxis;
	};
}

#endif    //__KINEMATIC_CHARACTER_CONTROLLER_HPP__