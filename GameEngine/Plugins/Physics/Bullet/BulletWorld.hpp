#pragma once

#include "BulletPlugin.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletWorld final : public WorldInterface
		{
		public:
			// Constructors
			BulletWorld(void) = delete;

			BulletWorld(BulletPlugin *plugin, const glm::vec3 &gravity, const std::string &worldName);

			BulletWorld(const BulletWorld &) = delete;

			// Assignment Operators
			BulletWorld &operator=(const BulletWorld &) = delete;

			// Destructor
			virtual ~BulletWorld(void) = default;

		private:
			// Attributes
			BulletPlugin *plugin = nullptr;

			// Inherited Methods
			void setGravity(const glm::vec3 &gravity) override final;

			glm::vec3 getGravity(void) const override final;

			void simulate(float stepSize) override final;
		};
	}
}
