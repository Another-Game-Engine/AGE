#pragma once

#include "PluginInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletPlugin final : public PluginInterface
		{
		public:
			// Constructors
			BulletPlugin(void) = default;

			BulletPlugin(const BulletPlugin &) = delete;

			// Assignment Operators
			BulletPlugin &operator=(const BulletPlugin &) = delete;

			// Destructor
			virtual ~BulletPlugin(void) = default;

		private:
			// Inherited Methods
			EngineTypes getPluginType(void) const override final;

			bool initialize(void) override final;

			void finalize(void) override final;

			WorldInterface *createWorld(const glm::vec3 &gravity, const std::string &worldName) override final;

			void deleteWorld(WorldInterface *world) override final;
		};
	}
}
