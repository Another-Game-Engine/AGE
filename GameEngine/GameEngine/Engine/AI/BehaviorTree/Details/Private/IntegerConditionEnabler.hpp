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
				template <typename IntegerType, class = typename std::enable_if<std::is_integral<IntegerType>::value && !std::is_same<IntegerType, bool>::value>::type>
				class IntegerConditionEnabler
				{
				public:
					// Constructors
					IntegerConditionEnabler(void) = default;

					IntegerConditionEnabler(IntegerConditionEnabler const &) = default;

					// Assignment Operators
					IntegerConditionEnabler &operator=(IntegerConditionEnabler const &) = default;

					// Destructor
					virtual ~IntegerConditionEnabler(void) = default;
				};
			}
		}
	}
}