#include <Systems/DebugSystem.hpp>
#include <Components/Light.hh>
#include <imgui/imgui.h>
#include <Core/Engine.hh>

namespace AGE
{
	DebugSystem::DebugSystem(AScene *scene) :
		System(std::move(scene)),
		_pointLights(std::move(scene))
	{
		_name = "Debug system";
		_timeMultiplier = GetEngine()->getTimeMultiplier();
	}

	bool DebugSystem::initialize()
	{
		_pointLights.requireComponent<PointLightComponent>();
		return (true);
	}

	void DebugSystem::updateBegin(float time)
	{
		ImGui::Begin("Debug system");
	}

	void DebugSystem::mainUpdate(float time)
	{
		ImGui::Text("Point light number : ", _pointLights.getCollection().size());

		if (ImGui::SliderFloat("Time multiplier", &_timeMultiplier, 0.0f, 10.0f))
		{
			GetEngine()->setTimeMultiplier(_timeMultiplier);
		}

	}

	void DebugSystem::updateEnd(float time)
	{
		ImGui::End();
	}
}