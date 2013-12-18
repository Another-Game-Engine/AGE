#ifndef		__RIGID_BODY_HPP__
#define		__RIGID_BODY_HPP__

#include <btBulletDynamicsCommon.h>
#include <Components/Component.hh>
#include "Utils/SmartPointer.hh"
#include <glm/glm.hpp>

namespace Component
{
	class RigidBody : public Component::ComponentBase<RigidBody>
	{
	public:
		RigidBody(Engine &engine, Handle &entity);
		virtual ~RigidBody(void);
	private:
		btCollisionShape *_collisionShape;
		btMotionState *_motionState;
		btRigidBody *_rigidBody;

		btScalar _mass;
		btVector3 _inertia;
	private:
		RigidBody(RigidBody const &);
		RigidBody &operator=(RigidBody const &);
	};

}

#endif //!__RIGID_BODY_HPP__