#include "SceneSelectorScene.hpp"
#include <imgui\imgui.h>
#include "AssetsEditorScene.hpp"
#include "WorldEditorScene.hpp"
namespace AGE
{
	const std::string SceneSelectorScene::Name = "SceneSelector";

	SceneSelectorScene::SceneSelectorScene(std::weak_ptr<AGE::Engine> engine)
		: AScene(engine)
	{
	}

	SceneSelectorScene::~SceneSelectorScene(void)
	{
	}

	bool SceneSelectorScene::userStart()
	{
		return true;
	}

	bool SceneSelectorScene::userUpdate(double time)
	{
		ImGui::Begin("Assets Convertor", (bool*)1, ImGui::GetIO().DisplaySize, 1, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);


		ImGui::BeginChild("Global Options", ImVec2(300, 0), true);
		if (ImGui::Button("Asset Editor"))
		{
			getEngine().lock()->disableScene(WorldEditorScene::Name);
			getEngine().lock()->enableScene(AssetsEditorScene::Name, 1000);
		}
		if (ImGui::Button("World Editor"))
		{
			getEngine().lock()->enableScene(WorldEditorScene::Name, 1000);
			getEngine().lock()->disableScene(AssetsEditorScene::Name);
		}
		ImGui::EndChild(); ImGui::SameLine();
		return true;
	}
}