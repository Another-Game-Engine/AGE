#pragma once

#include <Components/Component.hh>
#include <Utils/Serialization/SerializationArchives.hpp>

namespace AGE
{
	// Very basic FreeFly component for the moment.
	// It might contain more information in the future
	struct FreeFlyComponent : public ComponentBase
	{
		size_t notEmpty;

		void init();

		//////
		////
		// Serialization

		template <typename Archive>
		void serialize(Archive &ar)
		{
		}

		// !Serialization
		////
		//////
	};
}

