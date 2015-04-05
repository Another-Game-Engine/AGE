#pragma once

#include "PluginInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class Plugin final : public PluginInterface
		{
		public:
			// Constructors
			Plugin(void) = default;

			Plugin(const Plugin &) = default;

			// Assignment Operators
			Plugin &operator=(const Plugin &) = default;

			// Destructor
			virtual ~Plugin(void) = default;

		private:
			// Inherited Methods
			std::string getPluginName(void) const override final;
		};
	}
}
