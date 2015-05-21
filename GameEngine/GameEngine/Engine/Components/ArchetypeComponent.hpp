#pragma once

#include <Components/Component.hh>
#include <Utils/Serialization/SerializationArchives.hpp>

namespace AGE
{
	// Used to know if entity is archetype or not
	// An Archetype's entity will just have this component when serialized
	// At load, this component will be detected and the archetype's infos
	// will fill this entity
	struct ArchetypeComponent : public ComponentBase
	{
		AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_CORE_ArchetypeComponent");

		ArchetypeComponent()
		{
		}

		std::string archetypeName;

		void init(const std::string &_archetypeName = "AGE_INVALID_ARCHETYPE")
		{
			archetypeName = _archetypeName;
		}

		//////
		////
		// Serialization

		template <typename Archive>
		void serialize(Archive &ar, const std::uint32_t version)
		{
			ar(CEREAL_NVP(archetypeName));
		}

		virtual bool isExposedInEditor(){ return false; }

		// !Serialization
		////
		//////
	};
}

CEREAL_CLASS_VERSION(AGE::ArchetypeComponent, 0)