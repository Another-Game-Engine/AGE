#pragma once

#include <string>

#include <glm.hpp>

namespace AGE
{
	namespace Physics
	{
		class WorldInterface
		{
		public:
			// Constructors
			WorldInterface(void) = delete;

			inline WorldInterface(const std::string &worldName)
				: worldName(worldName)
			{
				return;
			}

			WorldInterface(const WorldInterface &) = delete;

			// Assignment Operators
			WorldInterface &operator=(const WorldInterface &) = delete;

			// Methods
			inline const std::string &getName(void) const
			{
				return worldName;
			}

			inline void setGravity(float x, float y, float z)
			{
				setGravity(glm::vec3(x, y, z));
			}

			inline void setTargetFPS(std::size_t target)
			{
				targetFPS = target;
			}

			inline std::size_t getTargetFPS(void) const
			{
				return targetFPS;
			}

			inline void update(float elapsedTime)
			{
				const float stepSize = 1.0f / static_cast<float>(targetFPS);
				accumulator += elapsedTime;
				while (accumulator >= stepSize)
				{
					simulate(stepSize);
					accumulator -= stepSize;
				}
			}

			// Virtual Methods
			virtual void setGravity(const glm::vec3 &gravity) = 0;

			virtual glm::vec3 getGravity(void) const = 0;

			virtual void simulate(float stepSize) = 0;

		protected:
			// Destructor
			virtual ~WorldInterface(void) = default;

		private:
			// Attributes
			std::string worldName;

			std::size_t targetFPS = 60;

			float accumulator = 0.0f;
		};
	}
}