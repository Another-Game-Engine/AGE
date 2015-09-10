#pragma once

#include <Components/Component.hh>
#include <Utils/Serialization/SerializationArchives.hpp>
#include <Utils/Debug.hpp>

namespace AGE
{
	// Used to know if entity is archetype or not
	// An Archetype's entity will just have this component when serialized
	// At load, this component will be detected and the archetype's infos
	// will fill this entity
	START_NOT_OPTIMIZED;

	struct ArchetypeComponent : public ComponentBase
	{
		AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_CORE_ArchetypeComponent");

		ArchetypeComponent();
		std::string archetypeName;
		bool parentIsAnArchetype = false;
#ifdef EDITOR_ENABLED
		bool synchronizePosition;
		bool synchronizeRotation;
		bool synchronizeScale;
#endif
		// never copied
		virtual void _copyFrom(const ComponentBase *model);
		void init(const std::string &_archetypeName = "AGE_INVALID_ARCHETYPE");

		//////
		////
		// Serialization
		template <typename Archive>
		void serialize(Archive &ar, const std::uint32_t version)
		{
			ar(CEREAL_NVP(archetypeName));
			if (version == 2)
			{
#ifdef EDITOR_ENABLED
				ar(synchronizePosition, synchronizeRotation, synchronizeScale, parentIsAnArchetype);
#endif
			}
			if (version == 3)
			{
#ifdef EDITOR_ENABLED
				ar(CEREAL_NVP(synchronizePosition), CEREAL_NVP(synchronizeRotation), CEREAL_NVP(synchronizeScale), CEREAL_NVP(parentIsAnArchetype));
#endif
			}
		}
		virtual void postUnserialization();
#ifdef EDITOR_ENABLED
		virtual bool isExposedInEditor();
		virtual bool editorUpdate();
#endif
		// !Serialization
		////
		//////
	};
	STOP_NOT_OPTIMIZED;
}

CEREAL_CLASS_VERSION(AGE::ArchetypeComponent, 3)