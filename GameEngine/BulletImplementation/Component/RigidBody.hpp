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

glm::vec3 posFromMat4(const glm::mat4 &mat)
{
	return glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
}

glm::vec3 scaleFromMat4(const glm::mat4 &m)
{
	// Extract col vectors of the matrix
	glm::vec3 col1(m[0][0], m[0][1], m[0][2]);
	glm::vec3 col2(m[1][0], m[1][1], m[1][2]);
	glm::vec3 col3(m[2][0], m[2][1], m[2][2]);
	//Extract the scaling factors
	glm::vec3 scaling;
	scaling.x = glm::length(col1);
	scaling.y = glm::length(col2);
	scaling.z = glm::length(col3);
	return scaling;
}

glm::vec3 rotFromMat4(const glm::mat4 &m, bool toEuler)
{
	glm::vec3 scale = scaleFromMat4(m);
	glm::vec3 columns[3] = {
		glm::vec3(m[0][0], m[0][1], m[0][2]),
		glm::vec3(m[1][0], m[1][1], m[1][2]),
		glm::vec3(m[2][0], m[2][1], m[2][2])
	};
	if (scale.x)
		columns[0] /= scale.x;
	if (scale.y)
		columns[1] /= scale.y;
	if (scale.z)
		columns[2] /= scale.z;
	glm::mat3 mat(columns[0].x, columns[1].x, columns[2].x,
		columns[0].y, columns[1].y, columns[2].y,
		columns[0].z, columns[1].z, columns[2].z);
	glm::quat quat(mat);
	glm::vec3 euler = glm::eulerAngles(quat);// ;
	if (toEuler)
		euler *= 3.14159f / 180.f;
	else
		euler *= -1;
	return euler;

}
namespace Component
{
	class RigidBody : public Component::ComponentBase<RigidBody>
	{
	public:

		typedef enum
		{
			SPHERE,
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

		RigidBody(Engine &engine, Handle &entity, float mass = 1.0f)
			: ComponentBase(engine, entity),
			_manager(engine.getInstance<BulletManager>()),
			_collisionShape(nullptr),
			_motionState(nullptr),
			_rigidBody(nullptr),
			_mass(mass),
			_inertia(btVector3(0.0f, 0.0f, 0.0f))
		{
		}

		btMotionState &getMotionState()
		{
			return *_motionState;
		}

		btCollisionShape &getShape()
		{
			return *_collisionShape;
		}

		btRigidBody &getBody()
		{
			return *_rigidBody;
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
			if (_rigidBody != nullptr)
			{
				_manager.getWorld().removeRigidBody(_rigidBody);
				delete _rigidBody;
			}
			if (_motionState != nullptr)
			{
				delete _motionState;
			}
			if (_collisionShape != nullptr)
			{
				delete _collisionShape;
			}
			btTransform transform;
			glm::vec3 position = posFromMat4(_entity->getLocalTransform());
			glm::vec3 scale = scaleFromMat4(_entity->getLocalTransform());
			glm::vec3 rot = rotFromMat4(_entity->getLocalTransform(), true);
			transform.setIdentity();
			transform.setOrigin(convertGLMVectorToBullet(position));
			transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));
			_motionState = new btDefaultMotionState(transform);
			btVector3 halfScale(scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f);
			if (c == CUBE)
				_collisionShape = new btBoxShape(halfScale);
			else if (c == SPHERE)
				_collisionShape = new btSphereShape(scale.x);
			//// get scale
			//glm::mat4 m = _entity->getLocalTransform();
			//// Extract col vectors of the matrix
			//glm::vec3 col1(m[0][0], m[0][1], m[0][2]);
			//glm::vec3 col2(m[1][0], m[1][1], m[1][2]);
			//glm::vec3 col3(m[2][0], m[2][1], m[2][2]);
			////Extract the scaling factors
			//glm::vec3 scaling;
			//scaling.x = glm::length(col1);
			//scaling.y = glm::length(col2);
			//scaling.z = glm::length(col3);

			//if (c == SPHERE)
			//{
			//	_collisionShape = new btSphereShape(1.0f);
			//}
			//else if (c == CUBE)
			//{
			//	_collisionShape = new btBoxShape(btVector3(0.5,0.5,0.5));
			//}
			//_collisionShape->setLocalScaling(btVector3(scaling.x, scaling.y, scaling.z));
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
			_rigidBody = new btRigidBody(_mass, _motionState, _collisionShape, _inertia);
			_rigidBody->setUserPointer(&_entity);

			_manager.getWorld().addRigidBody(_rigidBody);

			//if (_mass != 0)
			//	_collisionShape->calculateLocalInertia(_mass, _inertia);
			//btRigidBody::btRigidBodyConstructionInfo rbInfo(_mass, _motionState, _collisionShape, _inertia);
			//_rigidBody = new btRigidBody(rbInfo);

			////_rigidBody->setWorldTransform(convertGLMTransformToBullet(_entity.get()->getLocalTransform()));
			//btTransform t;
			//t = _rigidBody->getWorldTransform();
			//auto tr = _entity.get()->getLocalTransform();
			//t.setOrigin(btVector3(tr[3].x, tr[3].y, tr[3].z));
			//_manager.getWorld().addRigidBody(_rigidBody);
			//_rigidBody->setWorldTransform(t);
		}
	};

}

#endif //!__RIGID_BODY_HPP__