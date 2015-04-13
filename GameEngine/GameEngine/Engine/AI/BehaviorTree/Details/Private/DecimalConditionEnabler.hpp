#pragma once

#include <type_traits>

namespace AGE
{
	namespace AI
	{
		namespace Details
		{
			namespace Private
			{
				template < typename DecimalType, class = typename std::enable_if<std::is_floating_point<DecimalType>::value>::type>
				class DecimalConditionEnabler
				{
				public:
					// Constructors
					DecimalConditionEnabler(void) = default;

					DecimalConditionEnabler(DecimalConditionEnabler const &) = default;

					// Assignment Operators
					DecimalConditionEnabler &operator=(DecimalConditionEnabler const &) = default;

					// Destructor
					virtual ~DecimalConditionEnabler(void) = default;
				};
			}
		}
	}
}