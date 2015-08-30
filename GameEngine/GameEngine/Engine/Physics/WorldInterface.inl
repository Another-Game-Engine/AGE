#pragma once

#include <fstream>

#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/binary.hpp>

#include "WorldInterface.hpp"
#include "RigidBodyInterface.hpp"
#include "MaterialInterface.hpp"
#include "ColliderInterface.hpp"
#include "RaycasterInterface.hpp"
#include "../AssetManagement/AssetManager.hh"

namespace AGE
{
	namespace Physics
	{
		// Static Methods
		inline std::string WorldInterface::GetDefaultMaterialName(void)
		{
			return "DefaultMaterial";
		}

		inline glm::vec3 WorldInterface::GetDefaultGravity(void)
		{
			return glm::vec3(0.0f, -9.81f, 0.0f);
		}

		inline std::string WorldInterface::GetMaterialsFileName(void)
		{
			return "materials.page";
		}

		// Constructors
		inline WorldInterface::WorldInterface(PhysicsInterface *physics)
			: physics(physics)
		{
			filterNameToFilterGroup.emplace("GroupI", FilterGroup::GroupI);
			filterNameToFilterGroup.emplace("GroupII", FilterGroup::GroupII);
			filterNameToFilterGroup.emplace("GroupIII", FilterGroup::GroupIII);
			filterNameToFilterGroup.emplace("GroupIV", FilterGroup::GroupIV);
			filterNameToFilterGroup.emplace("GroupV", FilterGroup::GroupV);
			filterNameToFilterGroup.emplace("GroupVI", FilterGroup::GroupVI);
			filterNameToFilterGroup.emplace("GroupVII", FilterGroup::GroupVII);
			filterNameToFilterGroup.emplace("GroupVIII", FilterGroup::GroupVIII);
			filterNameToFilterGroup.emplace("GroupIX", FilterGroup::GroupIX);
			filterNameToFilterGroup.emplace("GroupX", FilterGroup::GroupX);
			filterNameToFilterGroup.emplace("GroupXI", FilterGroup::GroupXI);
			filterNameToFilterGroup.emplace("GroupXII", FilterGroup::GroupXII);
			filterNameToFilterGroup.emplace("GroupXIII", FilterGroup::GroupXIII);
			filterNameToFilterGroup.emplace("GroupXIV", FilterGroup::GroupXIV);
			filterNameToFilterGroup.emplace("GroupXV", FilterGroup::GroupXV);
			filterNameToFilterGroup.emplace("GroupXVI", FilterGroup::GroupXVI);
			filterNameToFilterGroup.emplace("GroupXVII", FilterGroup::GroupXVII);
			filterNameToFilterGroup.emplace("GroupXVIII", FilterGroup::GroupXVIII);
			filterNameToFilterGroup.emplace("GroupXIX", FilterGroup::GroupXIX);
			filterNameToFilterGroup.emplace("GroupXX", FilterGroup::GroupXX);
			filterNameToFilterGroup.emplace("GroupXXI", FilterGroup::GroupXXI);
			filterNameToFilterGroup.emplace("GroupXXII", FilterGroup::GroupXXII);
			filterNameToFilterGroup.emplace("GroupXXIII", FilterGroup::GroupXXIII);
			filterNameToFilterGroup.emplace("GroupXXIV", FilterGroup::GroupXXIV);
			filterNameToFilterGroup.emplace("GroupXXV", FilterGroup::GroupXXV);
			filterNameToFilterGroup.emplace("GroupXXVI", FilterGroup::GroupXXVI);
			filterNameToFilterGroup.emplace("GroupXXVII", FilterGroup::GroupXXVII);
			filterNameToFilterGroup.emplace("GroupXXVIII", FilterGroup::GroupXXVIII);
			filterNameToFilterGroup.emplace("GroupXXIX", FilterGroup::GroupXXIX);
			filterNameToFilterGroup.emplace("GroupXXX", FilterGroup::GroupXXX);
			filterNameToFilterGroup.emplace("GroupXXXI", FilterGroup::GroupXXXI);
			filterNameToFilterGroup.emplace("GroupXXXII", FilterGroup::GroupXXXII);
		}

		// Methods
		inline bool WorldInterface::initialize(AssetsManager *assetManager)
		{
			this->assetManager = assetManager;
			loadMaterials();
			raycaster = createRaycaster();
			assert(raycaster != nullptr && "Impossible to create raycaster");
			return raycaster != nullptr;
		}

		inline void WorldInterface::finalize(AssetsManager *assetManager)
		{
			this->assetManager = assetManager;
			destroyRaycaster();
			saveMaterials();
		}

		inline AssetsManager *WorldInterface::getAssetManager(void)
		{
			return assetManager;
		}

		inline const AssetsManager *WorldInterface::getAssetManager(void) const
		{
			return assetManager;
		}

		inline PhysicsInterface *WorldInterface::getPhysics(void)
		{
			return physics;
		}

		inline const PhysicsInterface *WorldInterface::getPhysics(void) const
		{
			return physics;
		}

		inline RaycasterInterface *WorldInterface::getRaycaster(void)
		{
			return raycaster;
		}

		inline const RaycasterInterface *WorldInterface::getRaycaster(void) const
		{
			return raycaster;
		}

		inline MaterialInterface *WorldInterface::getMaterial(const std::string &name)
		{
			MaterialTable::iterator found = materials.find(name);
			return found != materials.end() ? found->second.first : nullptr;
		}

		inline const MaterialInterface *WorldInterface::getMaterial(const std::string &name) const
		{
			MaterialTable::const_iterator found = materials.find(name);
			return found != materials.end() ? found->second.first : nullptr;
		}

		inline void WorldInterface::setCollisionListener(CollisionListener *listener)
		{
			collisionListener = listener;
		}

		inline CollisionListener *WorldInterface::getCollisionListener(void)
		{
			assert(collisionListener != nullptr && "Invalid collisionListener");
			return collisionListener;
		}

		inline const CollisionListener *WorldInterface::getCollisionListener(void) const
		{
			assert(collisionListener != nullptr && "Invalid collisionListener");
			return collisionListener;
		}

		inline void WorldInterface::setTriggerListener(TriggerListener *listener)
		{
			triggerListener = listener;
		}

		inline TriggerListener *WorldInterface::getTriggerListener(void)
		{
			assert(triggerListener != nullptr && "Invalid triggerListener");
			return triggerListener;
		}

		inline const TriggerListener *WorldInterface::getTriggerListener(void) const
		{
			assert(triggerListener != nullptr && "Invalid triggerListener");
			return triggerListener;
		}

		inline void WorldInterface::setGravity(float x, float y, float z)
		{
			setGravity(glm::vec3(x, y, z));
		}

		inline void WorldInterface::setTargetFPS(std::size_t target)
		{
			targetFPS = target;
		}

		inline std::size_t WorldInterface::getTargetFPS(void) const
		{
			return targetFPS;
		}

		inline void WorldInterface::update(float elapsedTime)
		{
			const float stepSize = 1.0f / static_cast<float>(targetFPS);
			accumulator += elapsedTime;
			while (accumulator >= stepSize)
			{
				simulate(stepSize);
				accumulator -= stepSize;
			}
		}

		inline void WorldInterface::setFilterNameForFilterGroup(FilterGroup group, const std::string &name)
		{
			assert(!name.empty() && "Invalid name");
			filterNameToFilterGroup.erase(getFilterNameForFilterGroup(group));
			filterNameToFilterGroup.emplace(name, group);
		}

		inline const std::string &WorldInterface::getFilterNameForFilterGroup(FilterGroup group) const
		{
			for (const HashTable::value_type &pair : filterNameToFilterGroup)
			{
				if (pair.second == group)
				{
					return pair.first;
				}
			}
			assert(!"Never reached");
			static std::string EmptyName;
			return EmptyName;
		}

		inline const FilterGroup WorldInterface::getFilterGroupForFilterName(const std::string &name) const
		{
			assert(!name.empty() && "Invalid name");
			HashTable::const_iterator found = filterNameToFilterGroup.find(name);
			assert(found != filterNameToFilterGroup.end() && "Name not found");
			return found != filterNameToFilterGroup.end() ? found->second : FilterGroup::GroupI;
		}

		inline void WorldInterface::enableCollisionBetweenGroups(const std::string &group1, const std::string &group2)
		{
			enableCollisionBetweenGroups(getFilterGroupForFilterName(group1), getFilterGroupForFilterName(group2));
		}

		inline void WorldInterface::disableCollisionBetweenGroups(const std::string &group1, const std::string &group2)
		{
			disableCollisionBetweenGroups(getFilterGroupForFilterName(group1), getFilterGroupForFilterName(group2));
		}

		inline void WorldInterface::saveMaterials(void)
		{
			const std::string path = assetManager->getAssetsDirectory() + GetMaterialsFileName();
			std::ofstream stream(path.c_str(), std::ios::binary);
			assert(stream.is_open() && "Impossible to open stream");
			if (!stream.is_open())
			{
				return;
			}
			cereal::BinaryOutputArchive archive(stream);
			archive(materials.size());
			for (MaterialTable::value_type &value : materials)
			{
				MaterialInterface *material = value.second.first;
				archive(material->getName());
				archive(material->getStaticFriction());
				archive(material->getDynamicFriction());
				archive(material->getRestitution());
			}
			while (!materials.empty())
			{
				destroyMaterial(materials.begin()->second.first);
			}
		}

		inline void WorldInterface::loadMaterials(void)
		{
			const std::string path = assetManager->getAssetsDirectory() + GetMaterialsFileName();
			std::ifstream stream(path.c_str(), std::ios::binary);
			if (!stream.is_open())
			{
				return;
			}
			cereal::BinaryInputArchive archive(stream);
			std::size_t size = 0;
			archive(size);
			for (std::size_t index = 0; index < size; ++index)
			{
				std::string name;
				float staticFriction;
				float dynamicFriction;
				float restitution;
				archive(name);
				archive(staticFriction);
				archive(dynamicFriction);
				archive(restitution);
				MaterialInterface *material = createMaterial(name);
				material->setStaticFriction(staticFriction);
				material->setDynamicFriction(dynamicFriction);
				material->setRestitution(restitution);
			}
		}

		inline void WorldInterface::destroyRaycaster(void)
		{
			assert(raycaster != nullptr && "Invalid raycaster");
			delete raycaster;
			raycaster = nullptr;
		}
	}
}