#include "SceneSelectorScene.hpp"
#include <imgui\imgui.h>
#include "AssetsEditorScene.hpp"
#include "SkyboxEditorScene.hh"
#include "WorldEditorScene.hpp"
#include <Core/Inputs/Input.hh>
#include <SDL/SDL.h>

namespace AGE
{
	const std::string SceneSelectorScene::Name = "SceneSelector";

	SceneSelectorScene::SceneSelectorScene(AGE::Engine *engine)
		: AScene(engine)
	{
	}

	SceneSelectorScene::~SceneSelectorScene(void)
	{
	}

	bool SceneSelectorScene::_userStart()
	{
		return true;
	}

	bool SceneSelectorScene::_userUpdateBegin(float time)
	{
		ImGui::Begin("Assets Convertor", (bool*)1, ImGui::GetIO().DisplaySize, 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);


		ImGui::BeginChild("Global Options", ImVec2(ImGui::GetIO().DisplaySize.x, 30), false);
		if (ImGui::Button("Asset Editor"))
		{
			getEngine()->disableScene(WorldEditorScene::Name);
			getEngine()->disableScene(SkyboxEditorScene::Name);
			getEngine()->enableScene(AssetsEditorScene::Name, 1000);
		}
		ImGui::SameLine();
		if (ImGui::Button("World Editor"))
		{
			getEngine()->enableScene(WorldEditorScene::Name, 1000);
			getEngine()->disableScene(AssetsEditorScene::Name);
			getEngine()->disableScene(SkyboxEditorScene::Name);
		}
		ImGui::SameLine();
		if (ImGui::Button("Skybox Editor"))
		{
			getEngine()->enableScene(SkyboxEditorScene::Name, 1000);
			getEngine()->disableScene(WorldEditorScene::Name);
			getEngine()->disableScene(AssetsEditorScene::Name);
		}
		ImGui::EndChild();
		return true;
	}

	bool SceneSelectorScene::_userUpdateEnd(float time)
	{
		if (getInstance<Input>()->getPhysicalKeyJustReleased(AGE_ESCAPE))
			return (false);
		return true;
	}
}