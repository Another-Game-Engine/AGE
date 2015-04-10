#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include <glm.hpp>

#include "EngineTypes.hpp"
#include "WorldInterface.hpp"
#include "../Utils/Dependency.hpp"

namespace AGE
{
	namespace Physics
	{
		class PluginInterface : public Dependency<PluginInterface>
		{
		public:
			// Constructors
			PluginInterface(void) = default;

			PluginInterface(const PluginInterface &) = delete;

			// Assignment Operators
			PluginInterface &operator=(const PluginInterface &) = delete;

			// Destructor
			virtual ~PluginInterface(void) = default;

			// Methods
			inline bool startup(void)
			{
				return initialize();
			}

			inline void shutdown(void)
			{
				finalize();
				for (const std::pair<const std::string, WorldInterface *> &pair : worldMap)
				{
					deleteWorld(pair.second);
				}
				worldMap.clear();
			}

			inline WorldInterface *createWorld(const std::string &worldName, float gravityX, float gravityY, float gravityZ)
			{
				return createWorld(worldName, glm::vec3(gravityX, gravityY, gravityZ));
			}

			inline WorldInterface *createWorld(const std::string &worldName, const glm::vec3 &gravity = glm::vec3(0.0f, -9.81f, 0.0f))
			{
				WorldInterface *world = createWorld(gravity, worldName);
				worldMap.emplace(worldName, world);
				return world;
			}

			inline WorldInterface *getWorldByName(const std::string &worldName)
			{
				WorldMapType::iterator found = worldMap.find(worldName);
				assert(found != worldMap.end() && "Invalid world name");
				if (found != worldMap.end())
				{
					return found->second;
				}
				else
				{
					return nullptr;
				}
			}

			inline void destroyWorld(const std::string &worldName)
			{
				destroyWorld(getWorldByName(worldName));
			}

			inline void destroyWorld(WorldInterface *world)
			{
				assert(world != nullptr && "Invalid world");
				if (world != nullptr)
				{
					deleteWorld(world);
					worldMap.erase(worldMap.find(world->getName()));
				}
			}

			// Virtual Methods
			virtual EngineTypes getPluginType(void) const = 0;

		protected:
			// Virtual Methods
			virtual bool initialize(void) = 0;

			virtual void finalize(void) = 0;

			virtual WorldInterface *createWorld(const glm::vec3 &gravity, const std::string &worldName) = 0;

			virtual void deleteWorld(WorldInterface *world) = 0;

		private:
			using WorldMapType = std::unordered_map < std::string, WorldInterface * >;

			// Attributes
			WorldMapType worldMap;
		};
	}
}