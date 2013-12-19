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


///
/// TO MOVE TO UTILS
///
glm::mat4 convertBulletTransformToGLM(const btTransform& transform)
{
	float data[16];
	transform.getOpenGLMatrix(data);
	return glm::make_mat4(data);
}
btTransform convertGLMTransformToBullet(const glm::mat4 &transform)
{
	const float* data = glm::value_ptr(transform);
	btTransform bulletTransform;
	bulletTransform.setFromOpenGLMatrix(data);
	return bulletTransform;
}
btVector3 convertGLMVectorToBullet(const glm::vec3 &vector)
{
	return btVector3(vector.x, vector.y, vector.z);
}
glm::vec3 convertBulletVectorToGLM(const btVector3& vector)
{
	return glm::vec3(vector.getX(), vector.getY(), vector.getZ());
}

namespace Component
{
	class RigidBody : public Component::ComponentBase<RigidBody>
	{
	public:

		typedef enum
		{
			SPHERE,
			PLANE,
			CUBE
		} CollisionShape;

		class EntityState : public btMotionState
		{
		public:
			EntityState(Handle &entity) :
				_entity(entity)
			{
				this->mWorldTrans = convertGLMTransformToBullet(entity->getLocalTransform());
			}

			~EntityState()
			{}

			virtual void getWorldTransform(btTransform& worldTrans) const
			{
				worldTrans = convertGLMTransformToBullet(_entity.get()->getLocalTransform());
			}
			virtual void setWorldTransform(const btTransform& worldTrans)
			{
				btTransform t = worldTrans;
				_entity.get()->setLocalTransform() = convertBulletTransformToGLM(t);
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
			//_motionState = new btDefaultMotionState(convertGLMTransformToBullet(_entity.get()->getLocalTransform())); //EntityState(entity);
			_motionState = new btDefaultMotionState(); //EntityState(entity);
		}

		btMotionState &getMotionState()
		{
			return *_motionState;
		}

		btCollisionShape &getShape()
		{
			return *_collisionShape;
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

			// get scale
			glm::mat4 m = _entity->getLocalTransform();
			// Extract col vectors of the matrix
			glm::vec3 col1(m[0][0], m[0][1], m[0][2]);
			glm::vec3 col2(m[1][0], m[1][1], m[1][2]);
			glm::vec3 col3(m[2][0], m[2][1], m[2][2]);
			//Extract the scaling factors
			glm::vec3 scaling;
			scaling.x = glm::length(col1);
			scaling.y = glm::length(col2);
			scaling.z = glm::length(col3);

			if (c == SPHERE)
				_collisionShape = new btSphereShape(1.0f);
			else if (c == PLANE)
				_collisionShape = new btBoxShape(btVector3(1,1,1));
			else if (c == CUBE)
			{
				_collisionShape = new btBoxShape(btVector3(0.5,0.5,0.5));
			}
			_collisionShape->setLocalScaling(btVector3(scaling.x, scaling.y, scaling.z));
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

			//_rigidBody->setWorldTransform(convertGLMTransformToBullet(_entity.get()->getLocalTransform()));
			btTransform t;
			t = _rigidBody->getWorldTransform();
			auto tr = _entity.get()->getLocalTransform();
			t.setOrigin(btVector3(tr[3].x, tr[3].y, tr[3].z));
			_manager.getWorld().addRigidBody(_rigidBody);
			_rigidBody->setWorldTransform(t);
		}
	};

}

#endif //!__RIGID_BODY_HPP__