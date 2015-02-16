#include "WorldEditorScene.hpp"
#include <imgui\imgui.h>
#include <Systems/EntityManager.hpp>

namespace AGE
{
	const std::string WorldEditorScene::Name = "WorldEditor";

	WorldEditorScene::WorldEditorScene(std::weak_ptr<AGE::Engine> engine)
		: AScene(engine)
	{
	}

	WorldEditorScene::~WorldEditorScene(void)
	{
	}

	bool WorldEditorScene::userStart()
	{
		addSystem<WE::EntityManager>(1);
		return true;
	}

	bool WorldEditorScene::userUpdateBegin(double time)
	{
		ImGui::BeginChild("Assets browser", ImVec2(ImGui::GetWindowWidth() * 0.2f, 0), true);
		ImGui::BeginChild("Raw", ImVec2(0, 0), false);
		return true;
	}

	bool WorldEditorScene::userUpdateEnd(double time)
	{
		ImGui::EndChild();
		ImGui::EndChild();
		ImGui::End();
		return true;
	}
}