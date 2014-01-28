#ifndef		__RIGID_BODY_HPP__
#define		__RIGID_BODY_HPP__

#include <btBulletDynamicsCommon.h>
#include <Components/Component.hh>
#include "Utils/SmartPointer.hh"
#include <Entities/EntityData.hh>
#include <Entities/Entity.hh>
#include <Core/Engine.hh>
#include <MediaFiles/ObjFile.hpp>
#include <Managers/BulletDynamicManager.hpp>
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include <hacdHACD.h>
#include <Utils/BtConversion.hpp>
#include <Utils/MatrixConversion.hpp>


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
			CONCAVE_STATIC_MESH,
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
				_manager->getWorld()->removeRigidBody(_rigidBody);
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
			std::cout << scale.x << " " << scale.y << " " << scale.z << std::endl;
			glm::vec3 rot = rotFromMat4(_entity->getLocalTransform(), true);
			transform.setIdentity();
			transform.setOrigin(convertGLMVectorToBullet(position));
			transform.setRotation(btQuaternion(rot.x, rot.y, rot.z));
			_motionState = new btDefaultMotionState(transform);
			if (c == BOX)
			{
				_collisionShape = new btBoxShape(btVector3(0.5, 0.5, 0.5));//new btBoxShape(halfScale);
			}
			else if (c == SPHERE)
			{
				_collisionShape = new btSphereShape(0.5);//new btSphereShape(scale.x);
			}
			else if (c == MESH)
			{
				// THERE IS SOME LEAKS BECAUSE THAT'S TEMPORARY
				auto mesh = AMediaFile::get<ObjFile>(meshName);
				auto group = new btCompoundShape();

				auto &geos = mesh->geometries;

				for (unsigned int i = 0; i < geos.size(); ++i)
				{
					auto &geo = geos[i]; // DIRTY HACK TEMPORARY
					// NEED TO REPLACE MESH BY MESH GROUP !
					btScalar *t = new btScalar[geo.vertices.size() * 3]();
					for (unsigned int it = 0; it < geo.vertices.size(); ++it)
					{
						t[it * 3] = geo.vertices[it].x;
						t[it * 3 + 1] = geo.vertices[it].y;
						t[it * 3 + 2] = geo.vertices[it].z;
					}
					btConvexHullShape *tmp = new btConvexHullShape(t, geo.vertices.size(), 3 * sizeof(btScalar));
					btShapeHull *hull = new btShapeHull(tmp);
					btScalar margin = tmp->getMargin();
					hull->buildHull(margin);
					tmp->setUserPointer(hull);
					btConvexHullShape *s = new btConvexHullShape();
					for (int it = 0; it < hull->numVertices(); ++it)
					{
						s->addPoint(hull->getVertexPointer()[it], false);
					}
					s->recalcLocalAabb();
					btTransform localTrans;
					localTrans.setIdentity();
					_collisionShape = s;
					group->addChildShape(localTrans,s);
					delete[] t;
					delete hull;
					delete tmp;
				}
				_collisionShape = group;
			}
			else if (c == CONCAVE_STATIC_MESH) // dont work
			{
				auto mesh = AMediaFile::get<ObjFile>(meshName);
				auto trimesh = new btTriangleMesh();
				auto &geos = mesh->geometries;

				for (unsigned int j = 0; j < geos.size(); ++j)
				{
					auto &geo = geos[j];
					for (unsigned int i = 2; i < geo.vertices.size(); i += 3)
					{
						trimesh->addTriangle(btVector3(geo.vertices[i - 2].x, geo.vertices[i - 2].y, geo.vertices[i - 2].z)
							, btVector3(geo.vertices[i - 1].x, geo.vertices[i - 1].y, geo.vertices[i - 1].z)
							, btVector3(geo.vertices[i].x, geo.vertices[i].y, geo.vertices[i].z));
					}
				}

				auto bvh = new btBvhTriangleMeshShape(trimesh, true);
				bvh->buildOptimizedBvh();
				bool isit = bvh->isConcave();
				_collisionShape = bvh;
			}
			if (mass != 0)
				_collisionShape->calculateLocalInertia(mass, inertia);
			_collisionShape->setLocalScaling(convertGLMVectorToBullet(scale));
			_rigidBody = new btRigidBody(mass, _motionState, _collisionShape, inertia);
			_rigidBody->setUserPointer(&_entity);
			_rigidBody->setAngularFactor(convertGLMVectorToBullet(rotationConstraint));
			_rigidBody->setLinearFactor(convertGLMVectorToBullet(transformConstraint));
			if (_rigidBody->isStaticObject())
			{
				_rigidBody->setActivationState(DISABLE_SIMULATION);
			}
			_manager->getWorld()->addRigidBody(_rigidBody);
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
			if (_rigidBody)
			{
				_manager->getWorld()->removeRigidBody(_rigidBody);
				delete _rigidBody;
			}
			if (_motionState)
				delete _motionState;
			if (_collisionShape)
				delete _collisionShape;
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
		btCollisionShape *_collisionShape;
		btMotionState *_motionState;
		btRigidBody *_rigidBody;
	private:
		RigidBody(RigidBody const &);
		RigidBody &operator=(RigidBody const &);

		void _reset()
		{
			if (_rigidBody != nullptr)
			{
				_manager->getWorld()->removeRigidBody(_rigidBody);
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