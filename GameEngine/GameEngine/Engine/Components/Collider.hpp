#pragma once

#include <Components/Component.hh>
#include <Physics/ColliderInterface.hpp>
#include <Physics/Collision.hpp>
#include <Physics/Trigger.hpp>

namespace AGE
{
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

	public:
		// Constructors
		Collider(void) = default;

		Collider(const Collider &) = delete;

		// Assignment Operators
		Collider &operator=(const Collider &) = delete;

		// Destructor
		~Collider(void) = default;

		// Methods
		void init(Physics::ColliderType colliderType);

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

		const std::vector<Physics::Collision> &getCollisions(void) const;

	private:
		// Attributes
		Physics::ColliderInterface *collider = nullptr;

		std::vector<Physics::Collision> collisions;

		std::vector<Physics::Trigger> triggers;

		// Inherited Methods
		void reset(void);
	};
}