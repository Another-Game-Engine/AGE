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
		// on ne devrait pas passer par la mais par le manager
		assert(false);
		ArchetypeComponent *m = (ArchetypeComponent *)model;
		archetypeName = m->archetypeName;
#ifdef EDITOR_ENABLED
		synchronizePosition = m->synchronizePosition;
		synchronizeRotation = m->synchronizeRotation;
		synchronizeScale = m->synchronizeScale;
		deletableInEditor = m->deletableInEditor;
#endif
		//entity->getLink()->setPosition(m->entity->getLink().getPosition());
		//entity->getLink()->setScale(m->entity->getLink().getScale());
		//entity->getLink()->setOrientation(m->entity->getLink().getOrientation());
	}

}