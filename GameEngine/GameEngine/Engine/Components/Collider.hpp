#pragma once

#include <string>

#include <Components/Component.hh>
#include <Physics/ColliderInterface.hpp>
#include <Physics/Collision.hpp>
#include <Physics/Trigger.hpp>
#include <Utils/Serialization/VectorSerialization.hpp>

namespace AGE
{
	struct Link;

	namespace Private
	{
		class CollisionSystem;
		class TriggerSystem;
	}

	class Collider final : public ComponentBase
	{
		// Friendships
		friend Private::CollisionSystem;

		friend Private::TriggerSystem;

		friend Link;

	public:
		// Constructors
		Collider(void) = default;

		Collider(const Collider &) = delete;

		// Assignment Operators
		Collider &operator=(const Collider &) = delete;

		// Destructor
		~Collider(void) = default;

		// Methods
		void init(Physics::ColliderType colliderType = Physics::ColliderType::Box, const std::string &mesh = "");

		void setStaticFriction(float staticFriction);

		float getStaticFriction(void) const;

		void setDynamicFriction(float dynamicFriction);

		float getDynamicFriction(void) const;

		void setRestitution(float restitution);

		float getRestitution(void) const;

		Physics::ColliderType getColliderType(void) const;

		void setAsTrigger(bool mustBeATrigger);

		bool isATrigger(void) const;

		void setFilterGroup(const std::string &filterName);

		void setFilterGroup(Physics::FilterGroup group);

		const std::string &getFilterGroupName(void) const;

		Physics::FilterGroup getFilterGroup(void) const;

		void setCenter(const glm::vec3 &center);

		glm::vec3 getCenter(void) const;

		void setHeight(float height);

		float getHeight(void) const;

		void setRadius(float radius);

		float getRadius(void) const;

		void setSize(const glm::vec3 &size);

		glm::vec3 getSize(void) const;

		void setMesh(const std::string &mesh);

		std::shared_ptr<MeshInstance> getMesh(void);

		std::shared_ptr<const MeshInstance> getMesh(void) const;

		void setAsConvex(bool mustBeConvex);

		bool isConvex(void) const;

		const std::vector<Physics::Collision> &getCollisions(void) const;

		template <typename Archive>
		void save(Archive &ar, const std::uint32_t version) const;

		template <typename Archive>
		void saveBoxCollider(Archive &ar, const std::uint32_t version) const;

		template <typename Archive>
		void saveCapsuleCollider(Archive &ar, const std::uint32_t version) const;

		template <typename Archive>
		void saveMeshCollider(Archive &ar, const std::uint32_t version) const;

		template <typename Archive>
		void saveSphereCollider(Archive &ar, const std::uint32_t version) const;

		template <typename Archive>
		void load(Archive &ar, const std::uint32_t version);

		template <typename Archive>
		void loadBoxCollider(Archive &ar, const std::uint32_t version);

		template <typename Archive>
		void loadCapsuleCollider(Archive &ar, const std::uint32_t version);

		template <typename Archive>
		void loadMeshCollider(Archive &ar, const std::uint32_t version);

		template <typename Archive>
		void loadSphereCollider(Archive &ar, const std::uint32_t version);

	private:
		// Attributes
		Physics::ColliderInterface *collider = nullptr;

		std::vector<Physics::Collision> collisions;

		std::vector<Physics::Trigger> triggers;

		// Methods
		void scale(const glm::vec3 &scaling);

		// Inherited Methods
		void reset(void);
	};
}

CEREAL_CLASS_VERSION(AGE::Collider, 1);

#include <Components/Collider.inl>