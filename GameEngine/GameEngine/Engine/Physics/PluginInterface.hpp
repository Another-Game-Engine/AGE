#pragma once

#include <string>

namespace AGE
{
	namespace Physics
	{
		class PluginInterface
		{
		public:
			// Constructors
			PluginInterface(void) = default;

			PluginInterface(const PluginInterface &) = default;

			// Assignment Operators
			PluginInterface &operator=(const PluginInterface &) = default;

			// Destructor
			virtual ~PluginInterface(void) = default;

			// Virtual Methods
			virtual std::string getPluginName(void) const = 0;
		};
	}
}