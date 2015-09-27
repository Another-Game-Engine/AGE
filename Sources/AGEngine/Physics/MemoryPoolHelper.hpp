#pragma once

#include <type_traits>

#include <Utils/ObjectPool.hpp>

namespace AGE
{
	namespace Physics
	{
		template <class RigidBodyType, class MaterialType, class BoxColliderType, class CapsuleColliderType, class MeshColliderType, class SphereColliderType, std::size_t Alignment = 16, std::size_t ObjectNumberPerChunk = 1024>
		class MemoryPoolHelper
		{
		public:
			// Constructors
			MemoryPoolHelper(void) = default;

			MemoryPoolHelper(const MemoryPoolHelper &) = delete;

			// Assignment Operators
			MemoryPoolHelper &operator=(const MemoryPoolHelper &) = delete;

			// Destructor
			virtual ~MemoryPoolHelper(void) = default;

			// Methods
			template <typename T, typename... Args, class Enable = typename std::enable_if<std::is_same<T, RigidBodyType>::value || std::is_same<T, MaterialType>::value || std::is_same<T, BoxColliderType>::value || std::is_same<T, CapsuleColliderType>::value || std::is_same<T, MeshColliderType>::value || std::is_same<T, SphereColliderType>::value>::type>
			T *create(Args &&...args);

			template <typename T, class Enable = typename std::enable_if<std::is_same<T, RigidBodyType>::value || std::is_same<T, MaterialType>::value || std::is_same<T, BoxColliderType>::value || std::is_same<T, CapsuleColliderType>::value || std::is_same<T, MeshColliderType>::value || std::is_same<T, SphereColliderType>::value>::type>
			void destroy(T *instance);

		private:
			// Helpers
			template <typename T>
			class PoolHelperBase
			{
			public:
				// Constructors
				PoolHelperBase(void) = delete;

				inline PoolHelperBase(ObjectPool<T, Alignment, ObjectNumberPerChunk> &pool)
					: pool(pool)
				{
					return;
				}

				PoolHelperBase(const PoolHelperBase &) = delete;

				// Assignment Operators
				PoolHelperBase &operator=(const PoolHelperBase &) = delete;

				// Destructor
				virtual ~PoolHelperBase(void) = default;

				// Methods
				template <typename... Args>
				inline T *create(Args &&...args)
				{
					return pool.create(std::forward<Args>(args)...);
				}

				inline void destroy(T *instance)
				{
					pool.destroy(instance);
				}

			private:
				// Attributes
				ObjectPool<T, Alignment, ObjectNumberPerChunk> &pool;
			};

			template <typename T>
			struct PoolHelper;

			template <>
			struct PoolHelper<RigidBodyType> final : public PoolHelperBase<RigidBodyType>
			{
			public:
				inline PoolHelper(MemoryPoolHelper &creator)
					: PoolHelperBase(creator.rigidBodyPool)
				{
					return;
				}
			};

			template <>
			struct PoolHelper<MaterialType> final : public PoolHelperBase<MaterialType>
			{
			public:
				inline PoolHelper(MemoryPoolHelper &creator)
					: PoolHelperBase(creator.materialPool)
				{
					return;
				}
			};

			template <>
			struct PoolHelper<BoxColliderType> final : public PoolHelperBase<BoxColliderType>
			{
			public:
				inline PoolHelper(MemoryPoolHelper &creator)
					: PoolHelperBase(creator.boxColliderPool)
				{
					return;
				}
			};

			template <>
			struct PoolHelper<CapsuleColliderType> final : public PoolHelperBase<CapsuleColliderType>
			{
			public:
				inline PoolHelper(MemoryPoolHelper &creator)
					: PoolHelperBase(creator.capsuleColliderPool)
				{
					return;
				}
			};

			template <>
			struct PoolHelper<MeshColliderType> final : public PoolHelperBase<MeshColliderType>
			{
			public:
				inline PoolHelper(MemoryPoolHelper &creator)
					: PoolHelperBase(creator.meshColliderPool)
				{
					return;
				}
			};

			template <>
			struct PoolHelper<SphereColliderType> final : public PoolHelperBase<SphereColliderType>
			{
			public:
				inline PoolHelper(MemoryPoolHelper &creator)
					: PoolHelperBase(creator.sphereColliderPool)
				{
					return;
				}
			};

			// Attributes
			ObjectPool<RigidBodyType, Alignment, ObjectNumberPerChunk> rigidBodyPool;

			ObjectPool<MaterialType, Alignment, ObjectNumberPerChunk> materialPool;

			ObjectPool<BoxColliderType, Alignment, ObjectNumberPerChunk> boxColliderPool;

			ObjectPool<CapsuleColliderType, Alignment, ObjectNumberPerChunk> capsuleColliderPool;

			ObjectPool<MeshColliderType, Alignment, ObjectNumberPerChunk> meshColliderPool;

			ObjectPool<SphereColliderType, Alignment, ObjectNumberPerChunk> sphereColliderPool;
		};
	}
}

#include "MemoryPoolHelper.inl"
