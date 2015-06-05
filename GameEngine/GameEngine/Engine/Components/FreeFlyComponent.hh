#pragma once

#include <Components/Component.hh>
#include <Utils/Serialization/SerializationArchives.hpp>

namespace AGE
{
	// Very basic FreeFly component for the moment.
	// It might contain more information in the future
	struct FreeFlyComponent : public ComponentBase
	{
		AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_CORE_FreeFlyComponent");
		size_t notEmpty;

		void init();

		//////
		////
		// Serialization

		template <typename Archive>
		void serialize(Archive &ar, const std::uint32_t version)
		{
		}

		// !Serialization
		////
		//////
	};
}

CEREAL_CLASS_VERSION(AGE::FreeFlyComponent, 0)