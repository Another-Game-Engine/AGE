#pragma once

#include "WorldInterface.hpp"
#include "RigidBodyInterface.hpp"
#include "MaterialInterface.hpp"
#include "ColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
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
		inline PhysicsInterface *WorldInterface::getPhysics(void)
		{
			return physics;
		}

		inline const PhysicsInterface *WorldInterface::getPhysics(void) const
		{
			return physics;
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

		inline void WorldInterface::destroyRigidBody(RigidBodyInterface *rigidBody)
		{
			delete rigidBody;
		}

		inline void WorldInterface::destroyCollider(ColliderInterface *collider)
		{
			delete collider;
		}

		inline void WorldInterface::destroyMaterial(MaterialInterface *material)
		{
			delete material;
		}
	}
}