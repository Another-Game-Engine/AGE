#ifndef __RIGID_BODY_HPP__
#define __RIGID_BODY_HPP__

#include <btBulletDynamicsCommon.h>
#include <Components/Component.hh>
#include <Entities/Entity.hh>
#include <Core/Engine.hh>
#include <Physic/BulletDynamicManager.hpp>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <HACD/hacdHACD.h>
#include <Physic/Utils/BtConversion.hpp>
#include <Utils/MatrixConversion.hpp>
#include <MediaFiles/CollisionShapeStaticFile.hpp>
#include <MediaFiles/CollisionShapeDynamicFile.hpp>
#include <MediaFiles/CollisionBoxFile.hpp>
#include <MediaFiles/CollisionSphereFile.hpp>
#include <memory>
#include <Components/CollisionLayers.hpp>
#include <Serialize/BulletWorldImporter/btBulletWorldImporter.h>
#include <MediaFiles/AssetsManager.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>

#include <Physic/DynamicMotionState.hpp>

namespace Component
{
	ATTRIBUTE_ALIGNED16(struct) RigidBody : public Component::ComponentBase<RigidBody>
	{
		enum CollisionShape
		{
			SPHERE,
			BOX,
			MESH,
			UNDEFINED
		};

		BT_DECLARE_ALIGNED_ALLOCATOR();
		RigidBody()
			: ComponentBase(),
			_shapeType(CollisionShape::UNDEFINED),
			_mass(0.0f),
			_inertia(btVector3(0.0f, 0.0f, 0.0f)),
			_rotationConstraint(glm::vec3(1, 1, 1)),
			_transformConstraint(glm::vec3(1, 1, 1)),
			_shapeName(""),
			_collisionShape(nullptr),
			_motionState(nullptr),
			_rigidBody(nullptr)
		{
		}

		void init(std::weak_ptr<AScene> scene, float mass = 1.0f)
		{
			_manager = std::dynamic_pointer_cast<BulletDynamicManager>(scene.lock()->getInstance<BulletCollisionManager>());
			assert(_manager.lock() != nullptr);
			_mass = mass;
		}

		virtual void reset()
		{
			if (_rigidBody != nullptr)
			{
				_manager.lock()->getWorld()->removeRigidBody(_rigidBody);
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

			_shapeType = CollisionShape::UNDEFINED;
			_mass = 0.0f;
			_inertia = btVector3(0.0f, 0.0f, 0.0f);
			_rotationConstraint = glm::vec3(1, 1, 1);
			_transformConstraint = glm::vec3(1, 1, 1);
		}

		void setTransformation(const glm::mat4 &transformation)
		{
			btTransform tt = _rigidBody->getCenterOfMassTransform();
			tt.setOrigin(convertGLMVectorToBullet(posFromMat4(transformation)));
			glm::quat rot = glm::quat(rotFromMat4(transformation, true));
			tt.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
			_rigidBody->setCenterOfMassTransform(tt);
			_rigidBody->setWorldTransform(tt);
			_collisionShape->setLocalScaling(convertGLMVectorToBullet(scaleFromMat4(transformation)));
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

		void setMass(float mass)
		{
			_mass = btScalar(mass);
		}

		void setInertia(const glm::vec3 &inertia)
		{
			_inertia = convertGLMVectorToBullet(inertia);
		}

		void setCollisionShape(std::weak_ptr<AScene> scene
			, const Entity &entity
			, CollisionShape c
			, const std::string &shapeName = "NULL"
			, short filterGroup = 1
			, short filterMask = -1)
		{
			if (c == UNDEFINED)
				return;

			if (_rigidBody != nullptr)
			{
				_manager.lock()->getWorld()->removeRigidBody(_rigidBody);
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

			_shapeName = shapeName;
			_shapeType = c;

			auto &entityTransform = scene.lock()->getTransform(entity);

			_motionState = new DynamicMotionState(scene.lock()->getTransformRef(entity));
			if (c == BOX)
			{
				_collisionShape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
			}
			else if (c == SPHERE)
			{
				_collisionShape = new btSphereShape(0.5);
			}
			else if (c == MESH)
			{
				auto mediaManager = scene.lock()->getInstance<AssetsManager>();
				auto media = mediaManager->get(_shapeName);
				if (!media)
					return;
				if (media->getType() == AMediaFile::COLLISION_SHAPE_DYNAMIC)
				{
					auto s = std::dynamic_pointer_cast<CollisionShapeDynamicFile>(mediaManager->get(_shapeName));
					_collisionShape = new btConvexHullShape(*s->shape.get());
				}
				else if (media->getType() == AMediaFile::COLLISION_SHAPE_STATIC)
				{
					auto s = std::dynamic_pointer_cast<CollisionShapeStaticFile>(mediaManager->get(_shapeName));
					_collisionShape = new btScaledBvhTriangleMeshShape(s->shape.get(), btVector3(1, 1, 1));
				}
				else if (media->getType() == AMediaFile::COLLISION_BOX)
				{
					auto s = std::dynamic_pointer_cast<CollisionBoxFile>(mediaManager->get(_shapeName));
					_collisionShape = new btBoxShape(*s->shape.get());
				}
				else if (media->getType() == AMediaFile::COLLISION_SPHERE)
				{
					auto s = std::dynamic_pointer_cast<CollisionSphereFile>(mediaManager->get(_shapeName));
					_collisionShape = new btSphereShape(*s->shape.get());
				}
				else
					assert(false && "Collision mesh not found.");
			}

			if (_mass != 0)
				_collisionShape->calculateLocalInertia(_mass, _inertia);
			_rigidBody = new btRigidBody(_mass, _motionState, _collisionShape, _inertia);
			_rigidBody->setUserPointer((void*)(scene.lock()->getEntityPtr(entity)));
			_rigidBody->setAngularFactor(convertGLMVectorToBullet(_rotationConstraint));
			_rigidBody->setLinearFactor(convertGLMVectorToBullet(_transformConstraint));

			if (_rigidBody->isStaticObject())
			{
				_rigidBody->setActivationState(DISABLE_SIMULATION);
			}
			_manager.lock()->getWorld()->addRigidBody(_rigidBody, filterGroup, filterMask);
			setTransformation(entityTransform);
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
			if (_rigidBody != nullptr)
			{
				_manager.lock()->getWorld()->removeRigidBody(_rigidBody);
				_rigidBody = nullptr;
			}
			if (_motionState != nullptr)
				_motionState = nullptr;
			if (_collisionShape != nullptr)
				_collisionShape = nullptr;
		}

		RigidBody(RigidBody &&o)
			: ComponentBase<RigidBody>(std::move(o))
		{
				_shapeType = std::move(o._shapeType);
				_mass = std::move(o._mass);
				_inertia = std::move(o._inertia);
				_rotationConstraint = std::move(o._rotationConstraint);
				_transformConstraint = std::move(o._transformConstraint);
				_shapeName = std::move(o._shapeName);
				_collisionShape = std::move(o._collisionShape);
				_motionState = std::move(o._motionState);
				_rigidBody = std::move(o._rigidBody);
				_manager = std::move(o._manager);
			}

		RigidBody &operator=(RigidBody &&o)
		{
			_shapeType = std::move(o._shapeType);
			_mass = std::move(o._mass);
			_inertia = std::move(o._inertia);
			_rotationConstraint = std::move(o._rotationConstraint);
			_transformConstraint = std::move(o._transformConstraint);
			_shapeName = std::move(o._shapeName);
			_collisionShape = std::move(o._collisionShape);
			_motionState = std::move(o._motionState);
			_rigidBody = std::move(o._rigidBody);
			_manager = std::move(o._manager);
			return *this;
		}


		//////
		////
		// Serialization

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


	private:
		std::weak_ptr<BulletDynamicManager> _manager;
		CollisionShape _shapeType;
		btScalar _mass;
		btVector3 _inertia;
		glm::vec3 _rotationConstraint;
		glm::vec3 _transformConstraint;
		std::string _shapeName;
		btCollisionShape *_collisionShape;
		btMotionState *_motionState;
		btRigidBody *_rigidBody;

	private:
		RigidBody &operator=(RigidBody const &o);
		RigidBody(RigidBody const &o);
	};
}

#endif //!__RIGID_BODY_HPP__