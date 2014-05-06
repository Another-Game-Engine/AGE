#ifndef		__COLLISION_BODY_HPP__
#define		__COLLISION_BODY_HPP__

#include <btBulletDynamicsCommon.h>
#include <Components/Component.hh>
#include <Entities/Entity.hh>
#include <Core/Engine.hh>
#include <Physic/BulletCollisionManager.hpp>
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include <Physic/Utils/BtConversion.hpp>
#include <Utils/MatrixConversion.hpp>
#include <Core/AScene.hh>
#include <Components/CollisionLayers.hpp>
#include <MediaFiles/AssetsManager.hpp>


namespace Component
{
	ATTRIBUTE_ALIGNED16(struct) CollisionBody : public Component::ComponentBase<CollisionBody>
	{
		BT_DECLARE_ALIGNED_ALLOCATOR();
		typedef enum
		{
			SPHERE,
			BOX,
			MESH,
			UNDEFINED
		} CollisionShape;

		CollisionBody()
			: ComponentBase(),
			_manager(nullptr),
			shapeType(UNDEFINED),
			meshName(""),
			_collisionShape(nullptr),
			_body(nullptr)
		{
		}

		CollisionBody &operator=(CollisionBody const &o)
		{
			_scene = o._scene;
			shapeType = o.shapeType;
			meshName = o.meshName;
			_manager = o._manager;
			_collisionShape = o._collisionShape;
			_body = o._body;
			return *this;
		}

		CollisionBody(CollisionBody const &o)
		{
			_scene = o._scene;
			shapeType = o.shapeType;
			meshName = o.meshName;
			_manager = o._manager;
			_collisionShape = o._collisionShape;
			_body = o._body;
		}

		void init(std::weak_ptr<AScene> scene, float _mass = 1.0f)
		{
			_scene = scene;
			_manager = std::dynamic_pointer_cast<BulletDynamicManager>(_scene.lock()->getInstance<BulletCollisionManager>());
			assert(_manager != nullptr);
		}

		virtual void reset()
		{
			if (_body != nullptr)
			{
				_manager->getWorld()->removeCollisionObject(_body.get());
				_body = nullptr;
			}
			if (_collisionShape != nullptr)
			{
				_collisionShape = nullptr;
			}
			shapeType = UNDEFINED;
		}


		btCollisionShape &getShape()
		{
			assert(_collisionShape != nullptr && "Shape is NULL, RigidBody error. Tips : Have you setAcollisionShape to Component ?.");
			return *_collisionShape;
		}

		btCollisionObject &getBody()
		{
			assert(_body != nullptr && "RigidBody is NULL. Tips : Have you setAcollisionShape to Component ?.");
			return *_body;
		}

		void setCollisionShape(const Entity &entity, CollisionShape c, const std::string &_meshName = "NULL", short filterGroup = 1, short filterMask = -1)
		{
			if (c == UNDEFINED)
				return;
			auto mediaManager = _scene.lock()->getInstance<AssetsManager>();
			meshName = _meshName;
			_reset();
			shapeType = c;
			btTransform transform;
			auto &entityTransform = _scene.lock()->getLocalTransform(entity);
			glm::vec3 position = posFromMat4(entityTransform);
			glm::vec3 scale = scaleFromMat4(entityTransform);
			glm::vec3 rot = rotFromMat4(entityTransform, true);
			transform.setIdentity();
			transform.setOrigin(convertGLMVectorToBullet(position));
			transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));

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
				auto media = mediaManager->get(_meshName);
				if (!media)
					return;
				if (media->getType() == AMediaFile::COLLISION_SHAPE_DYNAMIC)
				{
					auto s = std::dynamic_pointer_cast<CollisionShapeDynamicFile>(mediaManager->get(_meshName));
					_collisionShape = std::shared_ptr<btCollisionShape>(new btConvexHullShape(*s->shape.get()));
				}
				else if (media->getType() == AMediaFile::COLLISION_SHAPE_STATIC)
				{
					auto s = std::dynamic_pointer_cast<CollisionShapeStaticFile>(mediaManager->get(_meshName));
					_collisionShape = std::shared_ptr<btCollisionShape>(new btScaledBvhTriangleMeshShape(s->shape.get(), btVector3(1, 1, 1)));
				}
				else if (media->getType() == AMediaFile::COLLISION_BOX)
				{
					auto s = std::dynamic_pointer_cast<CollisionBoxFile>(mediaManager->get(_meshName));
					_collisionShape = std::shared_ptr<btCollisionShape>(new btBoxShape(*s->shape.get()));
				}
				else if (media->getType() == AMediaFile::COLLISION_SPHERE)
				{
					auto s = std::dynamic_pointer_cast<CollisionSphereFile>(mediaManager->get(_meshName));
					_collisionShape = std::shared_ptr<btCollisionShape>(new btSphereShape(*s->shape.get()));
				}
				else
					std::cerr << "Collision mesh not found." << std::endl;
			}
			_collisionShape->setLocalScaling(convertGLMVectorToBullet(scale));
			_body = std::shared_ptr<btCollisionObject>(new btCollisionObject());// (mass, _motionState.get(), _collisionShape.get(), inertia));
			_body->setCollisionShape(_collisionShape.get());
			_body->setUserIndex(entity.id);
			_manager->getWorld()->addCollisionObject(_body.get());
		}

		virtual ~CollisionBody(void)
		{
			if (_body != nullptr)
			{
				_manager->getWorld()->removeCollisionObject(_body.get());
				_body = nullptr;
			}
			if (_collisionShape != nullptr)
				_collisionShape = nullptr;
		}

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new CollisionBody();
			res->setEntity(e);
			res->init();
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
		std::weak_ptr<AScene> _scene;
		CollisionShape shapeType;
		std::string meshName;
		std::shared_ptr<BulletCollisionManager> _manager;
		std::shared_ptr<btCollisionShape> _collisionShape;
		std::shared_ptr<btCollisionObject> _body;

	private:

		void _reset()
		{
			if (_body != nullptr)
			{
				_manager->getWorld()->removeCollisionObject(_body.get());
				_body = nullptr;
			}
			if (_collisionShape != nullptr)
			{
				_collisionShape = nullptr;
			}
		}
	};
}

#endif //!__COLLISION_BODY_HPP__