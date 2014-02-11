#ifndef		__RIGID_BODY_HPP__
#define		__RIGID_BODY_HPP__

#include <btBulletDynamicsCommon.h>
#include <Components/Component.hh>
#include <Entities/EntityData.hh>
#include <Entities/Entity.hh>
#include <Core/Engine.hh>
#include <MediaFiles/ObjFile.hpp>
#include <Managers/BulletDynamicManager.hpp>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <HACD/hacdHACD.h>
#include <Utils/BtConversion.hpp>
#include <Utils/MatrixConversion.hpp>
#include <MediaFiles/CollisionShapeStaticFile.hpp>
#include <MediaFiles/CollisionShapeDynamicFile.hpp>
#include <MediaFiles/CollisionBoxFile.hpp>
#include <MediaFiles/CollisionSphereFile.hpp>
#include <memory>
#include <Components/CollisionLayers.hpp>
#include <Serialize/BulletWorldImporter/btBulletWorldImporter.h>

namespace Component
{
	ATTRIBUTE_ALIGNED16(struct) RigidBody : public Component::ComponentBase<RigidBody>
	{
		BT_DECLARE_ALIGNED_ALLOCATOR();
		typedef enum
		{
			SPHERE,
			BOX,
			MESH,
			UNDEFINED
		} CollisionShape;

		RigidBody()
			: ComponentBase(),
			_manager(nullptr),
			shapeType(UNDEFINED),
			mass(0.0f),
			inertia(btVector3(0.0f, 0.0f, 0.0f)),
			rotationConstraint(glm::vec3(1,1,1)),
			transformConstraint(glm::vec3(1,1,1)),
			meshName(""),
			_collisionShape(nullptr),
			_motionState(nullptr),
			_rigidBody(nullptr)
		{
		}

		void init(float _mass = 1.0f)
		{
			_manager = dynamic_cast<BulletDynamicManager*>(&_entity->getScene()->getEngine().getInstance<BulletCollisionManager>());
			assert(_manager != nullptr);
			mass = _mass;
		}

		virtual void reset()
		{
			if (_rigidBody != nullptr)
			{
				_manager->getWorld()->removeRigidBody(_rigidBody.get());
				_rigidBody = nullptr;
			}
			if (_motionState != nullptr)
			{
				_motionState = nullptr;
			}
			if (_collisionShape != nullptr)
			{
				_collisionShape = nullptr;
			}
			shapeType = UNDEFINED;
			mass = 0.0f;
			inertia = btVector3(0.0f, 0.0f, 0.0f);
			rotationConstraint = glm::vec3(1, 1, 1);
			transformConstraint = glm::vec3(1, 1, 1);
		}

		btMotionState &getMotionState()
		{
			assert(_motionState != nullptr && "Motion state is NULL, RigidBody error. Tips : Have you setAcollisionShape to Component ?.");
			return *_motionState;
		}

		btCollisionShape &getShape()
		{
			assert(_collisionShape != nullptr && "Shape is NULL, RigidBody error. Tips : Have you setAcollisionShape to Component ?.");
			return *_collisionShape;
		}

		btRigidBody &getBody()
		{
			assert(_rigidBody != nullptr && "RigidBody is NULL. Tips : Have you setAcollisionShape to Component ?.");
			return *_rigidBody;
		}

		void setMass(float _mass)
		{
			mass = btScalar(_mass);
		}

		void setInertia(btVector3 const &v)
		{
			inertia = v;
		}

		void setCollisionShape(CollisionShape c, const std::string &_meshName = "")
		{
			if (c == UNDEFINED)
				return;
			meshName = _meshName;
			_reset();
			shapeType = c;
			btTransform transform;
			glm::vec3 position = posFromMat4(_entity->getLocalTransform());
			glm::vec3 scale = scaleFromMat4(_entity->getLocalTransform());
			glm::vec3 rot = rotFromMat4(_entity->getLocalTransform(), true);
			transform.setIdentity();
			transform.setOrigin(convertGLMVectorToBullet(position));
			transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));

			_motionState = std::shared_ptr<btMotionState>(new btDefaultMotionState(transform));
			if (c == BOX)
			{
				_collisionShape = std::shared_ptr<btCollisionShape>(new btBoxShape(btVector3(0.5, 0.5, 0.5)));
			}
			else if (c == SPHERE)
			{
				_collisionShape = std::shared_ptr<btCollisionShape>(new btSphereShape(0.5));
			}
			else if (c == MESH)
			{
				auto media = AMediaFile::get(_meshName);
				if (!media)
					return;
				if (media->getType() == AMediaFile::COLLISION_SHAPE_DYNAMIC)
				{
					auto s = std::dynamic_pointer_cast<CollisionShapeDynamicFile>(AMediaFile::get(_meshName));
					_collisionShape = std::shared_ptr<btCollisionShape>(new btConvexHullShape(*s->shape.get()));
				}
				else if (media->getType() == AMediaFile::COLLISION_SHAPE_STATIC)
				{
					auto s = std::dynamic_pointer_cast<CollisionShapeStaticFile>(AMediaFile::get(_meshName));
					_collisionShape = std::shared_ptr<btCollisionShape>(new btScaledBvhTriangleMeshShape(s->shape.get(), btVector3(1, 1, 1)));
				}
				else if (media->getType() == AMediaFile::COLLISION_BOX)
				{
					auto s = std::dynamic_pointer_cast<CollisionBoxFile>(AMediaFile::get(_meshName));
					_collisionShape = std::shared_ptr<btCollisionShape>(new btBoxShape(*s->shape.get()));
				}
				else if (media->getType() == AMediaFile::COLLISION_SPHERE)
				{
					auto s = std::dynamic_pointer_cast<CollisionSphereFile>(AMediaFile::get(_meshName));
					_collisionShape = std::shared_ptr<btCollisionShape>(new btSphereShape(*s->shape.get()));
				}
				else
					std::cerr << "Collision mesh not found." << std::endl;
			}
			if (mass != 0)
				_collisionShape->calculateLocalInertia(mass, inertia);
			_collisionShape->setLocalScaling(convertGLMVectorToBullet(scale));
			_rigidBody = std::shared_ptr<btRigidBody>(new btRigidBody(mass, _motionState.get(), _collisionShape.get(), inertia));
			_rigidBody->setUserPointer(&_entity);
			_rigidBody->setAngularFactor(convertGLMVectorToBullet(rotationConstraint));
			_rigidBody->setLinearFactor(convertGLMVectorToBullet(transformConstraint));
			if (_rigidBody->isStaticObject())
			{
				_rigidBody->setActivationState(DISABLE_SIMULATION);
			}
			_manager->getWorld()->addRigidBody(_rigidBody.get());
		}

		void setRotationConstraint(bool x, bool y, bool z)
		{
			rotationConstraint = glm::vec3(static_cast<unsigned int>(x),
				static_cast<unsigned int>(y),
				static_cast<unsigned int>(z));
			if (!_rigidBody)
				return;
			_rigidBody->setAngularFactor(convertGLMVectorToBullet(rotationConstraint));
		}

		void setTransformConstraint(bool x, bool y, bool z)
		{
			transformConstraint = glm::vec3(static_cast<unsigned int>(x),
				static_cast<unsigned int>(y),
				static_cast<unsigned int>(z));
			if (!_rigidBody)
				return;
			_rigidBody->setLinearFactor(convertGLMVectorToBullet(transformConstraint));
		}

		virtual ~RigidBody(void)
		{
			if (_rigidBody != nullptr)
			{
				_manager->getWorld()->removeRigidBody(_rigidBody.get());
				_rigidBody = nullptr;
			}
			if (_motionState != nullptr)
				_motionState = nullptr;
			if (_collisionShape != nullptr)
				_collisionShape = nullptr;
		}

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new RigidBody();
			res->setEntity(e);
			ar(*res);
			return res;
		}

		template <typename Archive>
		void save(Archive &ar) const
		{
		}

		template <typename Archive>
		void load(Archive &ar)
		{
		}

		// !Serialization
		////
		//////

		BulletDynamicManager *_manager;
		CollisionShape shapeType;
		btScalar mass;
		btVector3 inertia;
		glm::vec3 rotationConstraint;
		glm::vec3 transformConstraint;
		std::string meshName;
		std::shared_ptr<btCollisionShape> _collisionShape;
		std::shared_ptr<btMotionState> _motionState;
		std::shared_ptr<btRigidBody> _rigidBody;
	private:
		RigidBody(RigidBody const &);
		RigidBody &operator=(RigidBody const &);

		void _reset()
		{
			if (_rigidBody != nullptr)
			{
				_manager->getWorld()->removeRigidBody(_rigidBody.get());
				_rigidBody = nullptr;
			}
			if (_motionState != nullptr)
			{
				_motionState = nullptr;
			}
			if (_collisionShape != nullptr)
			{
				_collisionShape = nullptr;
			}
		}
	};

}

#endif //!__RIGID_BODY_HPP__