#pragma once

#include <btBulletDynamicsCommon.h>
#include <Components/Component.hh>

class btCollisionShape;
class btMotionState;
class btRigidBody;

namespace AGE
{
	class BulletDynamicManager;

	ATTRIBUTE_ALIGNED16(struct) RigidBody : public ComponentBase
	{
		enum CollisionShape
		{
			SPHERE,
			BOX,
			MESH,
			UNDEFINED
		};

		BT_DECLARE_ALIGNED_ALLOCATOR();
		RigidBody();
		void init(AScene *scene, float mass = 1.0f);
		virtual void reset(AScene *scene);
		void setTransformation(const AGE::Link *link);
		btMotionState &getMotionState();
		btCollisionShape &getShape();
		btRigidBody &getBody();
		void setMass(float mass);
		int getMass() const;
		void setInertia(const glm::vec3 &inertia);
		void setCollisionMesh(std::weak_ptr<AScene> scene
			, const Entity &entity
			, const std::string &meshPath
			, short filterGroup = 1
			, short filterMask = -1);
		void setCollisionShape(
			std::weak_ptr<AScene> scene
			, const Entity &entity
			, CollisionShape c
			, short filterGroup = 1
			, short filterMask = -1);
		void setRotationConstraint(bool x, bool y, bool z);
		void setTransformConstraint(bool x, bool y, bool z);
		virtual ~RigidBody(void);
		RigidBody(RigidBody &&o);
		RigidBody &operator=(RigidBody &&o);
		//////
		////
		// Serialization

		template <typename Archive>
		void save(Archive &ar) const
		{
			std::string TODO = "todo";
			ar(TODO);
		}

		template <typename Archive>
		void load(Archive &ar)
		{
			std::string TODO;
			ar(TODO);
		}

		// !Serialization
		////
		//////


	private:
		btScalar _mass;
		btVector3 _inertia;
		glm::vec3 _rotationConstraint;
		glm::vec3 _transformConstraint;
		btCollisionShape *_collisionShape;
		btMotionState *_motionState;
		btRigidBody *_rigidBody;
		BulletDynamicManager *_manager;
		CollisionShape _shapeType;
		std::string _shapeName;

	private:
		RigidBody &operator=(RigidBody const &o);
		RigidBody(RigidBody const &o);
	};
}