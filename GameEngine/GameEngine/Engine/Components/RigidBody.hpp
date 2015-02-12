#pragma once

#include <btBulletDynamicsCommon.h>
#include <Components/Component.hh>

class btCollisionShape;
class btMotionState;
class btRigidBody;

namespace AGE
{
	class BulletDynamicManager;

	struct RigidBody : public ComponentBase
	{
		BT_DECLARE_ALIGNED_ALLOCATOR();
		enum CollisionShape
		{
			SPHERE,
			BOX,
			MESH,
			UNDEFINED
		};
	private:
		ATTRIBUTE_ALIGNED16(btCollisionShape *_collisionShape);
		ATTRIBUTE_ALIGNED16(btMotionState *_motionState);
		ATTRIBUTE_ALIGNED16(btRigidBody *_rigidBody);
		ATTRIBUTE_ALIGNED16(BulletDynamicManager *_manager);
		ATTRIBUTE_ALIGNED16(btScalar _mass);
		ATTRIBUTE_ALIGNED16(btVector3 _inertia);
		ATTRIBUTE_ALIGNED16(glm::vec3 _rotationConstraint);
		ATTRIBUTE_ALIGNED16(glm::vec3 _transformConstraint);
		ATTRIBUTE_ALIGNED16(CollisionShape _shapeType);
		ATTRIBUTE_ALIGNED16(std::string _shapeName);
	public:
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
		RigidBody &operator=(RigidBody const &o);
		RigidBody(RigidBody const &o);
	};
}