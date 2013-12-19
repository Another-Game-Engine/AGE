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

		typedef enum
		{
			SPHERE,
			PLANE
		} CollisionShape;

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
			_manager(engine.getInstance<BulletManager>()),
			_collisionShape(nullptr),
			_motionState(nullptr),
			_rigidBody(nullptr),
			_mass(1.0f),
			_inertia(btVector3(0.0f, 0.0f, 0.0f))
		{
			_mass = btScalar(1.0f);
			_inertia = btVector3(0.0f, 0.0f, 0.0f);
			_motionState = new EntityState(entity);
		}

		void setMass(float mass)
		{
			_mass = btScalar(mass);
			_init();
		}

		void setInertia(btVector3 const &v)
		{
			_inertia = v;
			_init();
		}

		void setCollisionShape(CollisionShape c)
		{
			if (_collisionShape != nullptr)
			{
				if (_rigidBody != nullptr)
				{
					_manager.getWorld().removeRigidBody(_rigidBody);
					delete _rigidBody;
				}
				delete _collisionShape;
			}
			if (c == SPHERE)
				_collisionShape = new btSphereShape(1.0f);
			else if (c == PLANE)
				_collisionShape = new btBoxShape(btVector3(50, 0.1, 50));
			_init();
		}

		virtual ~RigidBody(void)
		{
			if (_rigidBody)
			{
				_manager.getWorld().removeRigidBody(_rigidBody);
				delete _rigidBody;
			}
			if (_motionState)
				delete _motionState;
			if (_collisionShape)
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
		void _init()
		{
			if (!_collisionShape || !_motionState)
				return;
			if (_rigidBody != nullptr)
			{
				_manager.getWorld().removeRigidBody(_rigidBody);
				delete _rigidBody;
			}
			if (_mass != 0)
				_collisionShape->calculateLocalInertia(_mass, _inertia);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(_mass, _motionState, _collisionShape, _inertia);
			_rigidBody = new btRigidBody(rbInfo);
			_manager.getWorld().addRigidBody(_rigidBody);
		}
	};

}

#endif //!__RIGID_BODY_HPP__