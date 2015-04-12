#pragma once

#include <Components/Component.hh>
#include <Utils/UniqueTypeId.hpp>
#include <Utils/Serialization/btSerialization.hpp>
#ifdef EDITOR_ENABLED
#include <WorldEditorGlobal.hpp>
#endif
#include <Physics/ContactInformation.hpp>

class btCollisionShape;
class btMotionState;
class btRigidBody;

namespace AGE
{
	class BulletDynamicManager;

	struct RigidBody : public ComponentBase
	{
		friend class BulletDynamicSystem;

		using ContactInformationList = std::vector < ContactInformation >;
		using ContactInformationsType = std::unordered_map < Entity, ContactInformationList > ;

		enum CollisionShape
		{
			SPHERE = 0,
			BOX = 1,
			UNDEFINED = 2
		};

	private:
		btCollisionShape *_collisionShape;
		btMotionState *_motionState;
		btRigidBody *_rigidBody;
		BulletDynamicManager *_manager;
		btScalar _mass;
		btVector3 _inertia;
		glm::uvec3 _rotationConstraint;
		glm::uvec3 _transformConstraint;
		UniqueTypeId _shapeType;
		std::string _shapePath;
		CollisionShape _collisionShapeType;
		ContactInformationsType _contactInformations;

		void _clearBulletObjects();
		void _addContactInformation(const Entity &entity, const btVector3 &contactPoint, const btVector3 &contactNormal);

	public:
		RigidBody();
		void init(float mass = 1.0f);
		virtual void reset();
		void setTransformation(const AGE::Link *link);
		btMotionState &getMotionState();
		btCollisionShape &getShape();
		btRigidBody &getBody();
		void setMass(float mass);
		float getMass() const;
		void setInertia(const glm::vec3 &inertia);
		void setCollisionMesh(
			const std::string &meshPath
			, short filterGroup = 1
			, short filterMask = -1);
		void setCollisionShape(
			CollisionShape c
			, short filterGroup = 1
			, short filterMask = -1);
		void setRotationConstraint(bool x, bool y, bool z);
		void setTransformConstraint(bool x, bool y, bool z);
		const ContactInformationsType &getContactInformations(void) const;
		virtual ~RigidBody(void);
		RigidBody(RigidBody &&o) = delete;
		//////
		////
		// Serialization

		template <typename Archive>
		void serialize(Archive &ar, std::uint32_t const version)
		{
			ar(cereal::make_nvp("Mass", _mass));
			ar(cereal::make_nvp("Inertia", _inertia));
			ar(cereal::make_nvp("Rotation constraint", _rotationConstraint));
			ar(cereal::make_nvp("Transform constraint", _transformConstraint));
			ar(cereal::make_nvp("Collision shape path", _shapePath));
			ar(cereal::make_nvp("Collision shape type", _collisionShapeType));
#ifdef EDITOR_ENABLED
			if (WESerialization::SerializeForEditor() == true)
			{
				if (version > 0)
				{
					ar(cereal::make_nvp("Selected mesh shape index", selectedShapeIndex));
					ar(cereal::make_nvp("Selected mesh shape path", selectedShapePath));
					ar(cereal::make_nvp("Selected mesh shape name", selectedShapeName));
					ar(cereal::make_nvp("Simple shape", simpleShapes));
				}
			}
#endif
		}

		// !Serialization
		////
		//////

		virtual void postUnserialization();

#ifdef EDITOR_ENABLED
		std::vector<const char*> *shapeFileList = nullptr;
		std::vector<const char*> *shapePathList = nullptr;
		std::size_t selectedShapeIndex = 0;
		std::string selectedShapeName = "";
		std::string selectedShapePath = "";
		bool simpleShapes = true;

		virtual void editorCreate();
		virtual void editorDelete();
		virtual bool editorUpdate();
#endif
	protected:
		virtual void _copyFrom(const ComponentBase *model)
		{
			// !!! TODO TODO TODO
		}
	private:
		RigidBody(RigidBody const &o) = delete;
	};
}

CEREAL_CLASS_VERSION(AGE::RigidBody, 1);