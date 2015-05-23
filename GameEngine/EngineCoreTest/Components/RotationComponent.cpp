#include "RotationComponent.hpp"
#ifdef EDITOR_ENABLED
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#endif

namespace AGE
{
	RotationComponent::RotationComponent()
		: _angles(0)
	{

	}

	RotationComponent::~RotationComponent(void)
	{

	}

	void RotationComponent::_copyFrom(const ComponentBase *model)
	{
		auto o = static_cast<const RotationComponent*>(model);
		_speed = o->_speed;
		_angles = o->_angles;
	}


	void RotationComponent::init(const glm::vec3 &angles, float speed)
	{
		if (glm::length(angles) != 0)
		{
			_angles = glm::normalize(angles);
		}
		_speed = speed;
	}

	void RotationComponent::reset()
	{
		_angles = glm::vec3(0);
	}


#ifdef EDITOR_ENABLED
	void RotationComponent::editorUpdate(AScene *scene)
	{
		ImGui::SliderFloat3("Angle", glm::value_ptr(_angles), 0.0f, 1.0f);
		ImGui::InputFloat("Speed", &_speed);
	}
#endif
}