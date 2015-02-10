#include "RigidBody.hpp"
#include <Core/AScene.hh>
#include <Physic/BulletDynamicManager.hpp>
#include <Physic/DynamicMotionState.hpp>


namespace AGE
{
	RigidBody::RigidBody()
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

	void RigidBody::init(AScene *scene, float mass/* = 1.0f*/)
	{
		_manager = dynamic_cast<BulletDynamicManager*>(scene->getInstance<BulletCollisionManager>());
		assert(_manager != nullptr);
		_mass = mass;
	}

	void RigidBody::reset(AScene *scene)
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

		_shapeType = CollisionShape::UNDEFINED;
		_mass = 0.0f;
		_rotationConstraint = glm::vec3(1, 1, 1);
		_transformConstraint = glm::vec3(1, 1, 1);
	}

	void RigidBody::setTransformation(const AGE::Link *link)
	{
		btTransform tt = _rigidBody->getCenterOfMassTransform();
		tt.setOrigin(convertGLMVectorToBullet(link->getPosition()));
		glm::quat rot = link->getOrientation();
		tt.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
		_rigidBody->setWorldTransform(tt);
		_collisionShape->setLocalScaling(convertGLMVectorToBullet(link->getScale()));
	}

	btMotionState &RigidBody::getMotionState()
	{
		assert(_motionState != nullptr && "Motion state is NULL, RigidBody error. Tips : Have you setAcollisionShape to Component ?.");
		return *_motionState;
	}

	btCollisionShape &RigidBody::getShape()
	{
		assert(_collisionShape != nullptr && "Shape is NULL, RigidBody error. Tips : Have you setAcollisionShape to Component ?.");
		return *_collisionShape;
	}

	btRigidBody &RigidBody::getBody()
	{
		assert(_rigidBody != nullptr && "RigidBody is NULL. Tips : Have you setAcollisionShape to Component ?.");
		return *_rigidBody;
	}

	void RigidBody::setMass(float mass)
	{
		_mass = btScalar(mass);
	}

	int RigidBody::getMass() const
	{
		return _mass;
	}

	void RigidBody::setInertia(const glm::vec3 &inertia)
	{
		_inertia = convertGLMVectorToBullet(inertia);
	}

	void RigidBody::setCollisionMesh(std::weak_ptr<AScene> scene
		, const Entity &entity
		, const std::string &meshPath
		, short filterGroup /*= 1*/
		, short filterMask /*= -1*/)
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

		_shapeName = meshPath;
		_shapeType = MESH;

		auto e = entity;
		_motionState = new DynamicMotionState(&e.getLink());
		auto media = _manager->loadShape(meshPath);
		if (!media)
			return;
		auto s = dynamic_cast<btConvexHullShape*>(media.get());
		if (s) // dynamic
		{
			_collisionShape = new btConvexHullShape(*s);
		}
		else // static
		{
			auto m = dynamic_cast<btBvhTriangleMeshShape*>(media.get());
			_collisionShape = new btScaledBvhTriangleMeshShape(m, btVector3(1, 1, 1));
		}

		if (_mass != 0)
			_collisionShape->calculateLocalInertia(_mass, _inertia);
		_rigidBody = new btRigidBody(_mass, _motionState, _collisionShape, _inertia);
		_rigidBody->setUserPointer((void*)(entity.getPtr()));
		_rigidBody->setAngularFactor(convertGLMVectorToBullet(_rotationConstraint));
		_rigidBody->setLinearFactor(convertGLMVectorToBullet(_transformConstraint));

		if (_rigidBody->isStaticObject())
		{
			//_rigidBody->setActivationState(DISABLE_SIMULATION);
		}
		_manager->getWorld()->addRigidBody(_rigidBody, filterGroup, filterMask);
		setTransformation(&e.getLink());
	}

	void RigidBody::setCollisionShape(
		std::weak_ptr<AScene> scene
		, const Entity &entity
		, CollisionShape c
		, short filterGroup /*= 1*/
		, short filterMask /* = -1*/)
	{
		if (c == UNDEFINED)
			return;
		if (c == MESH)
		{
			std::cerr << "Error : use setCollisionMesh instead of setCollisionShape" << std::endl;
			return;
		}
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
		_shapeType = c;
		auto e = entity;
		_motionState = new DynamicMotionState(&e.getLink());
		if (c == BOX)
		{
			_collisionShape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
		}
		else if (c == SPHERE)
		{
			_collisionShape = new btSphereShape(1);
		}
		else
		{
			assert(false);
		}

		if (_mass != 0)
			_collisionShape->calculateLocalInertia(_mass, _inertia);
		_rigidBody = new btRigidBody(_mass, _motionState, _collisionShape, _inertia);
		_rigidBody->setUserPointer((void*)(entity.getPtr()));
		_rigidBody->setAngularFactor(convertGLMVectorToBullet(_rotationConstraint));
		_rigidBody->setLinearFactor(convertGLMVectorToBullet(_transformConstraint));

		if (_rigidBody->isStaticObject())
		{
			//_rigidBody->setActivationState(DISABLE_SIMULATION);
		}
		_manager->getWorld()->addRigidBody(_rigidBody, filterGroup, filterMask);
		setTransformation(&e.getLink());
	}

	void RigidBody::setRotationConstraint(bool x, bool y, bool z)
	{
		_rotationConstraint = glm::vec3(static_cast<unsigned int>(x),
			static_cast<unsigned int>(y),
			static_cast<unsigned int>(z));
		if (!_rigidBody)
			return;
		_rigidBody->setAngularFactor(convertGLMVectorToBullet(_rotationConstraint));
	}

	void RigidBody::setTransformConstraint(bool x, bool y, bool z)
	{
		_transformConstraint = glm::vec3(static_cast<unsigned int>(x),
			static_cast<unsigned int>(y),
			static_cast<unsigned int>(z));
		if (!_rigidBody)
			return;
		_rigidBody->setLinearFactor(convertGLMVectorToBullet(_transformConstraint));
	}

	RigidBody::~RigidBody(void)
	{
	}

	RigidBody::RigidBody(RigidBody &&o)
		: ComponentBase(std::move(o))
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

	RigidBody &RigidBody::operator=(RigidBody &&o)
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
}
