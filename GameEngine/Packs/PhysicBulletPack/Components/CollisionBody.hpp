#ifndef		__COLLISION_BODY_HPP__
#define		__COLLISION_BODY_HPP__

#include <btBulletDynamicsCommon.h>
#include <Components/Component.hh>
#include "Utils/SmartPointer.hh"
#include <Entities/Entity.hh>
#include <Entities/Handle.hh>
#include <Core/Engine.hh>
#include <ResourceManager/ResourceManager.hh>
#include <ResourceManager/SharedMesh.hh>
#include <Managers/BulletCollisionManager.hpp>
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include <Utils/BtConversion.hpp>
#include <Utils/MatrixConversion.hpp>


namespace Component
{
	ATTRIBUTE_ALIGNED16(class) CollisionBody : public Component::ComponentBase<CollisionBody>
	{
	public:
		BT_DECLARE_ALIGNED_ALLOCATOR();
		typedef enum
		{
			SPHERE,
			BOX,
			MESH,
			UNDEFINED
		} CollisionShape;

		CollisionBody(Engine &engine, Handle &entity)
			: ComponentBase(engine, entity, "CollisionBody"),
			_manager(nullptr),
			_shapeType(UNDEFINED),
			_meshName(""),
			_collisionShape(nullptr),
			_body(nullptr)
		{
			_manager = dynamic_cast<BulletCollisionManager*>(&engine.getInstance<BulletCollisionManager>());
			assert(_manager != nullptr);
		}

		virtual void reset()
		{
			if (_body != nullptr)
			{
				_manager->getWorld()->removeCollisionObject(_body);
				delete _body;
				_body = nullptr;
			}
			if (_collisionShape != nullptr)
			{
				delete _collisionShape;
				_collisionShape = nullptr;
			}
			_shapeType = UNDEFINED;
		}

		bool init()
		{
			return true;
		}

		btCollisionShape &getShape()
		{
			assert(_collisionShape != nullptr && "Shape is NULL, RigidBody error. Tips : Have you setAcollisionShape to Component ?.");
			return *_collisionShape;
		}

		btCollisionObject &getBody()
		{
			assert(_body != nullptr && "Collision body is NULL. Tips : Have you setAcollisionShape to Component ?.");
			return *_body;
		}

		void setCollisionShape(CollisionShape c, const std::string &meshName = "")
		{
			if (c == UNDEFINED)
				return;
			_meshName = meshName;
			_reset();
			_shapeType = c;
			btTransform transform;
			glm::vec3 position = posFromMat4(_entity->getLocalTransform());
			glm::vec3 scale = scaleFromMat4(_entity->getLocalTransform());
			std::cout << scale.x << " " << scale.y << " " << scale.z << std::endl;
			glm::vec3 rot = rotFromMat4(_entity->getLocalTransform(), true);
			transform.setIdentity();
			transform.setOrigin(convertGLMVectorToBullet(position));
			transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));
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
			_body = new btCollisionObject();
			_body->setUserPointer(&_entity);
			_body->setCollisionShape(_collisionShape);
			_manager->getWorld()->addCollisionObject(_body);
		}

		virtual ~CollisionBody(void)
		{
			if (_body)
			{
				_manager->getWorld()->removeCollisionObject(_body);
				delete _body;
			}
			if (_collisionShape)
				delete _collisionShape;
		}

	private:
		BulletCollisionManager *_manager;
		CollisionShape _shapeType;
		std::string _meshName;
		btCollisionShape *_collisionShape;
		btCollisionObject *_body;
	private:
		CollisionBody(CollisionBody const &);
		CollisionBody &operator=(CollisionBody const &);

		void _reset()
		{
			if (_body != nullptr)
			{
				_manager->getWorld()->removeCollisionObject(_body);
				delete _body;
			}
			if (_collisionShape != nullptr)
			{
				delete _collisionShape;
			}
		}
	};

}

#endif //!__COLLISION_BODY_HPP__