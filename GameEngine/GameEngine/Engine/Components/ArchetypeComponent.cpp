#include "ArchetypeComponent.hpp"
#include <imgui/imgui.h>

namespace AGE
{
	ArchetypeComponent::ArchetypeComponent()
	{
	}

#ifdef EDITOR_ENABLED
	bool ArchetypeComponent::isExposedInEditor(){ return true; }
	bool ArchetypeComponent::editorUpdate()
	{
		bool modified = false;
		modified |= ImGui::Checkbox("Sync Position", &synchronizePosition);
		modified |= ImGui::Checkbox("Sync Rotation", &synchronizeRotation);
		modified |= ImGui::Checkbox("Sync Scale", &synchronizeScale);
		return modified;
	}
#endif

	void ArchetypeComponent::init(const std::string &_archetypeName)
	{
		archetypeName = _archetypeName;
#ifdef EDITOR_ENABLED
		synchronizePosition = false;
		synchronizeRotation = false;
		synchronizeScale = false;
		deletableInEditor = false;
#endif
	}

	void ArchetypeComponent::postUnserialization()
	{
		init(archetypeName);
	}

	void ArchetypeComponent::_copyFrom(const ComponentBase *model)
	{
		(void)(model);
	}

}