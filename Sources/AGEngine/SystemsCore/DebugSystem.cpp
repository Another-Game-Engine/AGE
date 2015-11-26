#include <SystemsCore/DebugSystem.hpp>
#include <ComponentsCore/Light.hh>
#include <imgui/imgui.h>
#include <Core/Engine.hh>
#include <Core/AScene.hh>
#include <SystemsCore/PhysicsSystem.hpp>
#include <Context/IRenderContext.hh>
#include <Core/ConfigurationManager.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/ThreadManager.hpp>

namespace AGE
{
	DebugSystem::DebugSystem(AScene *scene) :
		System(std::move(scene)),
		_pointLights(std::move(scene))
	{
		_name = "Debug system";
		_timeMultiplier = GetEngine()->getTimeMultiplier();
		_windowWidth = _scene->getInstance<ConfigurationManager>()->getConfiguration<int>("windowW")->value;
		_windowHeight = _scene->getInstance<ConfigurationManager>()->getConfiguration<int>("windowH")->value;
		_fullscreen = _scene->getInstance<ConfigurationManager>()->getConfiguration<bool>("fullScreen")->value;
		_frameCap = _scene->getInstance<ConfigurationManager>()->getConfiguration<size_t>("frameCap")->value;
	}

	bool DebugSystem::initialize()
	{
		_pointLights.requireComponent<PointLightComponent>();
		return (true);
	}

	void DebugSystem::updateBegin(float time)
	{
#if defined(AGE_ENABLE_IMGUI)
		ImGui::Begin("Debug system");
#endif
	}

	void DebugSystem::mainUpdate(float time)
	{
#if defined(AGE_ENABLE_IMGUI)
		ImGui::Text("Point light number : %i", _pointLights.getCollection().size());

		if (ImGui::SliderFloat("Time multiplier", &_timeMultiplier, 0.0f, 10.0f))
		{
			GetEngine()->setTimeMultiplier(_timeMultiplier);
		}

		static bool hasPhysics = _scene->getSystem<AGE::PhysicsSystem>() == nullptr ? true : false;
		if (ImGui::Checkbox("Physics activated", &hasPhysics))
		{
			if (hasPhysics)
			{
				_scene->activateSystem<AGE::PhysicsSystem>();
			}
			else
			{
				_scene->deactivateSystem<AGE::PhysicsSystem>();
			}
		}
		ImGui::Separator();
		ImGui::Text("Options : applied after relaunch.");
		auto context = _scene->getInstance<IRenderContext>();
		if (ImGui::Checkbox("Fullscreen", &_fullscreen))
		{
			_scene->getInstance<ConfigurationManager>()->setValue<bool>(std::string("fullScreen"), _fullscreen);
		}
		if (ImGui::SliderInt("Window width", &_windowWidth, 800, 1920))
		{
			_scene->getInstance<ConfigurationManager>()->setValue<int>(std::string("windowW"), _windowWidth);
		}
		if (ImGui::SliderInt("Window height", &_windowHeight, 600, 1080))
		{
			_scene->getInstance<ConfigurationManager>()->setValue<int>(std::string("windowH"), _windowHeight);
		}
		static int frameCap = _frameCap;
		bool changeFrameCap = false;
		if (ImGui::RadioButton("30 fps", &frameCap, 1000000 / 30))
			changeFrameCap = true;
		ImGui::SameLine();
		if (ImGui::RadioButton("60 fps", &frameCap, 1000000 / 60))
			changeFrameCap = true;
		ImGui::SameLine();
		if (ImGui::RadioButton("120 fps", &frameCap, 1000000 / 120))
			changeFrameCap = true;
		ImGui::SameLine();
		if (ImGui::RadioButton("Max fps", &frameCap, 0))
			changeFrameCap = true;
		if (changeFrameCap)
		{
			GetMainThread()->setFrameCap(frameCap);
			_scene->getInstance<ConfigurationManager>()->setValue<size_t>(std::string("frameCap"), frameCap);
		}

#endif
	}

	void DebugSystem::updateEnd(float time)
	{
#if defined(AGE_ENABLE_IMGUI)
		ImGui::End();
#endif
	}
}