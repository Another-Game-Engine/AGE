#pragma once

#include <Physics/PluginInterface.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullPlugin final : public PluginInterface
		{
		public:
			// Constructors
			NullPlugin(void) = default;

			NullPlugin(const NullPlugin &) = delete;

			// Assignment Operators
			NullPlugin &operator=(const NullPlugin &) = delete;

			// Destructor
			virtual ~NullPlugin(void) = default;

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
