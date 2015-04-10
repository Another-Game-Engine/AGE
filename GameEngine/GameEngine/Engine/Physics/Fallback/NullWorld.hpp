#pragma once

#include <Physics/WorldInterface.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullWorld final : public WorldInterface
		{
		public:
			// Constructors
			NullWorld(void) = delete;

			NullWorld(const std::string &worldName);

			NullWorld(const NullWorld &) = delete;

			// Assignment Operators
			NullWorld &operator=(const NullWorld &) = delete;

			// Destructor
			virtual ~NullWorld(void) = default;

		private:
			// Inherited Methods
			void setGravity(const glm::vec3 &gravity) override final;

			glm::vec3 getGravity(void) const override final;

			void simulate(float stepSize) override final;
		};
	}
}
