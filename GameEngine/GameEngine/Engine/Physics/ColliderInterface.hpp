#pragma once

#include "ColliderType.hpp"
#include "FilterGroup.hpp"

namespace AGE
{
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
			friend WorldInterface;

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

			ColliderInterface(WorldInterface *world, void *&data);

			ColliderInterface(const ColliderInterface &) = delete;

			// Assignment Operators
			ColliderInterface &operator=(const ColliderInterface &) = delete;

			// Methods
			WorldInterface *getWorld(void);

			const WorldInterface *getWorld(void) const;

			MaterialInterface *getMaterial(void);

			const MaterialInterface *getMaterial(void) const;

			void *&getData(void);

			void * const &getData(void) const;

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

		private:
			// Attributes
			WorldInterface *world = nullptr;

			void *&data;

			MaterialInterface *material = nullptr;
		};
	}
}

#include "ColliderInterface.inl"