#pragma once

#include <string>

#include <glm/glm.hpp>

#include "ColliderType.hpp"
#include "FilterGroup.hpp"
#include "GenericData.hpp"

namespace AGE
{
	class Collider;

	namespace Physics
	{
		class WorldInterface;
		class MaterialInterface;
		class BoxColliderInterface;
		class SphereColliderInterface;
		class CapsuleColliderInterface;
		class MeshColliderInterface;

		class ColliderInterface
		{
			// Friendships
			friend Collider;

		private:
			// Type Traits
			template <ColliderType>
			struct DeduceColliderType;

			template <>
			struct DeduceColliderType<ColliderType::Box> final
			{
				using Type = BoxColliderInterface;
			};

			template <>
			struct DeduceColliderType<ColliderType::Capsule> final
			{
				using Type = CapsuleColliderInterface;
			};

			template <>
			struct DeduceColliderType<ColliderType::Mesh> final
			{
				using Type = MeshColliderInterface;
			};

			template <>
			struct DeduceColliderType<ColliderType::Sphere> final
			{
				using Type = SphereColliderInterface;
			};

		public:
			// Constructors
			ColliderInterface(void) = delete;

			ColliderInterface(WorldInterface *world, Private::GenericData *data);

			ColliderInterface(const ColliderInterface &) = delete;

			// Assignment Operators
			ColliderInterface &operator=(const ColliderInterface &) = delete;

			// Methods
			Collider *getCollider(void);

			const Collider *getCollider(void) const;

			WorldInterface *getWorld(void);

			const WorldInterface *getWorld(void) const;

			void setMaterial(MaterialInterface *newMaterial);

			MaterialInterface *getMaterial(void);

			const MaterialInterface *getMaterial(void) const;

			Private::GenericData *getData(void);

			const Private::GenericData *getData(void) const;

			template <typename T>
			T *getDataAs(void);

			template <typename T>
			const T *getDataAs(void) const;

			template <ColliderType Type>
			bool is(void) const;

			bool is(ColliderType colliderType) const;

			template <ColliderType Type>
			typename DeduceColliderType<Type>::Type *as(void);

			template <ColliderType Type>
			const typename DeduceColliderType<Type>::Type *as(void) const;

			// Virtual Methods
			virtual ColliderType getColliderType(void) const = 0;

			virtual void setAsTrigger(bool mustBeATrigger) = 0;

			virtual bool isATrigger(void) const = 0;

			virtual void setFilterGroup(FilterGroup group) = 0;

			virtual FilterGroup getFilterGroup(void) const = 0;

		protected:
			// Destructor
			virtual ~ColliderInterface(void);

			// Static Methods
			static bool IsTriggerByDefault(void);

			static FilterGroup GetDefaultFilterGroup(void);

			// Virtual Methods
			virtual void scale(const glm::vec3 &scaling) = 0;

			virtual void setMaterial(const std::string &name) = 0;

		private:
			// Attributes
			Collider *collider = nullptr;

			WorldInterface *world = nullptr;

			Private::GenericData *data;

			MaterialInterface *material = nullptr;
		};
	}
}

#include "ColliderInterface.inl"