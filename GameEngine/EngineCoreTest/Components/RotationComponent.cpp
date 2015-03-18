#include "RotationComponent.hpp"
#ifdef EDITOR_ENABLED
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#endif

namespace AGE
{
	RotationComponent::RotationComponent()
	{

	}

	RotationComponent::~RotationComponent(void)
	{

	}


	void RotationComponent::init(const glm::vec3 &angles)
	{
		_angles = glm::normalize(angles);
	}

	void RotationComponent::reset()
	{
		_angles = glm::vec3(0);
	}


#ifdef EDITOR_ENABLED
	void RotationComponent::editorUpdate(AScene *scene)
	{
		ImGui::SliderFloat3("Angle", glm::value_ptr(_angles), 0.0f, 1.0f);
	}
#endif
}