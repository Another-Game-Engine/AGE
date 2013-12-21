#ifndef		__RIGID_BODY_HPP__
#define		__RIGID_BODY_HPP__

#include <btBulletDynamicsCommon.h>
#include <Components/Component.hh>
#include "Utils/SmartPointer.hh"
#include <glm/glm.hpp>
#include <Entities/Entity.hh>
#include <Entities/Handle.hh>
#include <Core/Engine.hh>
#include <ResourceManager/ResourceManager.hh>
#include <ResourceManager/SharedMesh.hh>
#include "../BulletManager.hpp"
#include "BulletCollision/CollisionShapes/btShapeHull.h"


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
			BOX,
			MESH,
			UNDEFINED
		} CollisionShape;

		RigidBody(Engine &engine, Handle &entity)
			: ComponentBase(engine, entity),
			_manager(engine.getInstance<BulletManager>()),
			_collisionShape(nullptr),
			_motionState(nullptr),
			_rigidBody(nullptr),
			_shapeType(UNDEFINED),
			_mass(0.0f),
			_inertia(btVector3(0.0f, 0.0f, 0.0f)),
			_rotationConstraint(glm::vec3(1,1,1)),
			_transformConstraint(glm::vec3(1,1,1))
		{
		}

		void init(float mass = 1.0f)
		{
			_mass = mass;
		}

		virtual void reset()
		{
			if (_rigidBody != nullptr)
			{
				_manager.getWorld().removeRigidBody(_rigidBody);
				delete _rigidBody;
				_rigidBody = nullptr;
			}
			if (_motionState != nullptr)
			{
				delete _motionState;
				_motionState = nullptr;
			}
			if (_collisionShape != nullptr)
			{
				delete _collisionShape;
				_collisionShape = nullptr;
			}
			_shapeType = UNDEFINED;
			_mass = 0.0f;
			_inertia = btVector3(0.0f, 0.0f, 0.0f);
			_rotationConstraint = glm::vec3(1, 1, 1);
			_transformConstraint = glm::vec3(1, 1, 1);
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
		}

		void setInertia(btVector3 const &v)
		{
			_inertia = v;
		}

		void setCollisionShape(CollisionShape c, const std::string &meshName = "")
		{
			if (c == UNDEFINED)
				return;
			_reset();
			_shapeType = c;
			btTransform transform;
			glm::vec3 position = posFromMat4(_entity->getLocalTransform());
			glm::vec3 scale = scaleFromMat4(_entity->getLocalTransform());
			glm::vec3 rot = rotFromMat4(_entity->getLocalTransform(), true);
			transform.setIdentity();
			transform.setOrigin(convertGLMVectorToBullet(position));
			transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));
			_motionState = new btDefaultMotionState(transform);
			if (c == BOX)
			{
				btVector3 halfScale(scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f);
				_collisionShape = new btBoxShape(btVector3(1, 1, 1));//new btBoxShape(halfScale);
				_collisionShape->setLocalScaling(halfScale);
			}
			else if (c == SPHERE)
			{
				_collisionShape = new btSphereShape(1);//new btSphereShape(scale.x);
				_collisionShape->setLocalScaling(convertGLMVectorToBullet(scale));
			}
			else if (c == MESH)
			{
				SmartPointer<Resources::SharedMesh> mesh = _engine.getInstance<Resources::ResourceManager>().getResource(meshName);
				const Resources::Geometry &geo = mesh->getGeometry();
				btScalar *t = new btScalar[geo.vertices.size() * 3]();
				for (unsigned int i = 0; i < geo.vertices.size(); ++i)
				{
					t[i * 3] = geo.vertices[i].x;
					t[i * 3 + 1] = geo.vertices[i].y;
					t[i * 3 + 2] = geo.vertices[i].z;
				}
				btConvexHullShape *tmp = new btConvexHullShape(t, geo.vertices.size(), 3 * sizeof(btScalar));
				btShapeHull *hull = new btShapeHull(tmp);
				btScalar margin = tmp->getMargin();
				hull->buildHull(margin);
				tmp->setUserPointer(hull);
				btConvexHullShape *s = new btConvexHullShape();
				for (int i = 0; i < hull->numVertices(); ++i)
				{
					s->addPoint(hull->getVertexPointer()[i], false);
				}
				s->recalcLocalAabb();
				s->setLocalScaling(convertGLMVectorToBullet(scale));
				_collisionShape = s;
				delete t;
				delete hull;
				delete tmp;
			}
			if (_mass != 0)
				_collisionShape->calculateLocalInertia(_mass, _inertia);
			_rigidBody = new btRigidBody(_mass, _motionState, _collisionShape, _inertia);
			_rigidBody->setUserPointer(&_entity);
			_rigidBody->setAngularFactor(convertGLMVectorToBullet(_rotationConstraint));
			_rigidBody->setLinearFactor(convertGLMVectorToBullet(_transformConstraint));
			_manager.getWorld().addRigidBody(_rigidBody);
		}


		void updateScale()
		{
			if (!_collisionShape)
				return;
			if (_shapeType == BOX)
				_collisionShape->setLocalScaling(convertGLMVectorToBullet(scaleFromMat4(_entity->getLocalTransform()) / 2.0f));
			else if (_shapeType == SPHERE)
				_collisionShape->setLocalScaling(convertGLMVectorToBullet(scaleFromMat4(_entity->getLocalTransform())));
		}

		void setRotationConstraint(bool x, bool y, bool z)
		{
			_rotationConstraint = glm::vec3(static_cast<unsigned int>(x),
				static_cast<unsigned int>(y),
				static_cast<unsigned int>(z));
			if (!_rigidBody)
				return;
			_rigidBody->setAngularFactor(convertGLMVectorToBullet(_rotationConstraint));
		}

		void setTransformConstraint(bool x, bool y, bool z)
		{
			_transformConstraint = glm::vec3(static_cast<unsigned int>(x),
				static_cast<unsigned int>(y),
				static_cast<unsigned int>(z));
			if (!_rigidBody)
				return;
			_rigidBody->setLinearFactor(convertGLMVectorToBullet(_transformConstraint));
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
		CollisionShape _shapeType;
		btScalar _mass;
		btVector3 _inertia;
		glm::vec3 _rotationConstraint;
		glm::vec3 _transformConstraint;
	private:
		RigidBody(RigidBody const &);
		RigidBody &operator=(RigidBody const &);

		void _reset()
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
		}
	};

}

#endif //!__RIGID_BODY_HPP__