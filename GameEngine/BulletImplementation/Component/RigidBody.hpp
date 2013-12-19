#ifndef		__RIGID_BODY_HPP__
#define		__RIGID_BODY_HPP__

#include <btBulletDynamicsCommon.h>
#include <Components/Component.hh>
#include "Utils/SmartPointer.hh"
#include <glm/glm.hpp>
#include <Entities/Entity.hh>
#include <Entities/Handle.hh>
#include <Core/Engine.hh>
#include "../BulletManager.hpp"

namespace Component
{
	class RigidBody : public Component::ComponentBase<RigidBody>
	{
	public:
		class EntityState : public btMotionState
		{
		public:
			EntityState(Handle &entity) :
				_entity(entity)
			{}

			~EntityState()
			{}
    	
			virtual void getWorldTransform(btTransform& worldTrans) const
			{
				 worldTrans.setFromOpenGLMatrix(glm::value_ptr(_entity.get()->setLocalTransform()));
			}
			virtual void setWorldTransform(const btTransform& worldTrans)
			{
				 worldTrans.getOpenGLMatrix(glm::value_ptr(_entity.get()->setLocalTransform()));
			}
		private:
			Handle      _entity;
			btTransform  mWorldTrans;
		};

		RigidBody(Engine &engine, Handle &entity)
			: ComponentBase(engine, entity),
			_manager(engine.getInstance<BulletManager>())
		{
			_collisionShape = new btSphereShape(btScalar(1.0f));
			_mass = btScalar(1.0f);
			_inertia = btVector3(0.0f, 0.0f, 0.0f);
			_motionState = new EntityState(entity);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(_mass, _motionState, _collisionShape, _inertia);
			_rigidBody = new btRigidBody(rbInfo);
			_manager.getWorld().addRigidBody(_rigidBody);
		}

		virtual ~RigidBody(void)
		{
			_manager.getWorld().removeRigidBody(_rigidBody);
			delete _rigidBody;
			delete _motionState;
			delete _collisionShape;
		}		

	private:
		BulletManager &_manager;
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