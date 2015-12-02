#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "../Utils/Dependency.hpp"
#include "FilterGroup.hpp"
#include "ColliderType.hpp"
#include "MemoryPoolHelper.hpp"
#include "GenericData.hpp"
#include "CollisionListener.hpp"
#include "TriggerListener.hpp"
#include "CharacterControllerInterface.hh"
#include <AssetManagement/Instance/MeshInstance.hh>

namespace AGE
{
	class RigidBody;
	class Collider;
	class DebugDrawManager;

	namespace Physics
	{
		class PhysicsInterface;
		class RigidBodyInterface;
		class MaterialInterface;
		class ColliderInterface;
		class RaycasterInterface;

		class WorldInterface
		{
			// Friendship
			friend RigidBody;

			friend Collider;

			friend PhysicsInterface;

		public:
			// Static Methods
			static std::string GetDefaultMaterialName(void);

			// Constructors
			WorldInterface(void) = delete;

			WorldInterface(PhysicsInterface *physics);

			WorldInterface(const WorldInterface &) = delete;

			// Assignment Operators
			WorldInterface &operator=(const WorldInterface &) = delete;

			// Methods
			AssetsManager *getAssetManager(void);

			const AssetsManager *getAssetManager(void) const;

			PhysicsInterface *getPhysics(void);

			const PhysicsInterface *getPhysics(void) const;

			RaycasterInterface *getRaycaster(void);

			const RaycasterInterface *getRaycaster(void) const;

			MaterialInterface *getMaterial(const std::string &name);

			const MaterialInterface *getMaterial(const std::string &name) const;

			void setCollisionListener(CollisionListener *listener);

			CollisionListener *getCollisionListener(void);

			const CollisionListener *getCollisionListener(void) const;

			void setTriggerListener(TriggerListener *listener);

			TriggerListener *getTriggerListener(void);

			const TriggerListener *getTriggerListener(void) const;

			void setGravity(float x, float y, float z);

			void setTargetFPS(std::size_t target);

			std::size_t getTargetFPS(void) const;

			virtual void enableDebug(void);

			virtual void disableDebug(void);

			bool isDebugEnabled(void) const;

			void update(float elapsedTime);

			void setFilterNameForFilterGroup(FilterGroup group, const std::string &name);

			const std::string &getFilterNameForFilterGroup(FilterGroup group) const;

			const FilterGroup getFilterGroupForFilterName(const std::string &name) const;

			void enableCollisionBetweenGroups(const std::string &group1, const std::string &group2);

			void disableCollisionBetweenGroups(const std::string &group1, const std::string &group2);

			// Virtual Methods
			virtual void setGravity(const glm::vec3 &gravity) = 0;

			virtual glm::vec3 getGravity(void) const = 0;

			virtual void enableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2) = 0;

			virtual void disableCollisionBetweenGroups(FilterGroup group1, FilterGroup group2) = 0;

			virtual MaterialInterface *createMaterial(const std::string &name = GetDefaultMaterialName()) = 0;

			virtual void destroyMaterial(MaterialInterface *material) = 0;

			virtual CharacterControllerInterface *createCharacterController() = 0;

			virtual void destroyCharacterController(CharacterControllerInterface *cc) = 0;

			virtual void fillDebugInformation(DebugDrawManager *debugDrawManager) = 0;

		protected:
			// Type Aliases
			using MaterialTable = std::unordered_map < std::string, std::pair < MaterialInterface *, std::atomic_uint32_t > >;

			using ShapeTable = std::unordered_map < std::string, std::pair < void *, std::atomic_uint32_t > >;

			// Attributes
			MaterialTable materials;

			ShapeTable convexShapes;

			ShapeTable concaveShapes;

			// Destructor
			virtual ~WorldInterface(void) = default;

			// Static Methods
			static glm::vec3 GetDefaultGravity(void);

		private:
			// Type Aliases
			using HashTable = std::unordered_map < std::string, FilterGroup > ;

			// Attributes
			AssetsManager *assetManager;

			PhysicsInterface *physics = nullptr;

			RaycasterInterface *raycaster = nullptr;

			CollisionListener *collisionListener = nullptr;

			TriggerListener *triggerListener = nullptr;

			std::size_t targetFPS = 60;

			float accumulator = 0.0f;

			bool debugEnabled = false;

			HashTable filterNameToFilterGroup;

			// Static Methods
			static std::string GetMaterialsFileName(void);

			// Methods
			bool initialize(AssetsManager *assetManager);

			void finalize(AssetsManager *assetManager);

			void saveMaterials(void);

			void loadMaterials(void);

			void destroyRaycaster(void);

			// Virtual Methods
			virtual RaycasterInterface *createRaycaster(void) = 0;

			virtual RigidBodyInterface *createRigidBody(Private::GenericData *data) = 0;

			virtual void destroyRigidBody(RigidBodyInterface *rigidBody) = 0;

			virtual ColliderInterface *createCollider(ColliderType colliderType, const std::string &mesh, Private::GenericData *data) = 0;

			virtual void destroyCollider(ColliderInterface *collider) = 0;

			virtual void simulate(float stepSize) = 0;
		};
	}
}

#include "WorldInterface.inl"
