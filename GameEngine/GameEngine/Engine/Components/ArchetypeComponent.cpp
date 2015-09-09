#include "ArchetypeComponent.hpp"
#include <imgui/imgui.h>
#include "Entities\IArchetypeManager.hpp"
#include "Entities\Entity.hh"
#include "Core/AScene.hh"

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
		if (modified)
		{
			entity->getScene()->getInstance<AGE::IArchetypeManager>()->updateArchetype(archetypeName);
		}
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