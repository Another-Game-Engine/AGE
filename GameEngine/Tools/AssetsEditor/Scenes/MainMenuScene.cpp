#include "MainMenuScene.hpp"
#include <imgui\imgui.h>
#include "AssetsEditorScene.hpp"
#include "WorldEditorScene.hpp"
#include "MaterialEditorScene.hh"

namespace AGE
{
	const std::string MainMenuScene::Name = "Main menu";

	MainMenuScene::MainMenuScene(AGE::Engine *engine)
		: AScene(engine)
	{
	}

	MainMenuScene::~MainMenuScene(void)
	{
	}

	bool MainMenuScene::_userStart()
	{
		_selectedTool = WorldEditor;
		//getEngine()->enableScene(AGE::WorldEditorScene::Name, 1000);

		// we register tools names
		_toolsName[WorldEditor] = WorldEditorScene::Name;
		_toolsName[MaterialEditor] = MaterialEditorScene::Name;
		_toolsName[AssetsEditor] = AssetsEditorScene::Name;
		
		return true;
	}

	bool MainMenuScene::_userUpdateBegin(float time)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu(_toolsName[_selectedTool].c_str()))
			{
				for (auto i = 0; i < TOOLS_NUMBER; ++i)
				{
					bool notAlreadyActive = ToolType(i) != _selectedTool;

					if (notAlreadyActive && ImGui::MenuItem(_toolsName[i].c_str()))
					{
						getEngine()->disableScene(_toolsName[_selectedTool]);
						_selectedTool = ToolType(i);
						getEngine()->enableScene(_toolsName[_selectedTool], 1000);
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}


		ImGui::Begin("Assets Convertor"/*, (bool*)1, ImGui::GetIO().DisplaySize, 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar*/);


		//ImGui::BeginChild("Global Options", ImVec2(ImGui::GetIO().DisplaySize.x, 30), false);
		//if (ImGui::Button("Asset Editor"))
		//{
		//	getEngine()->disableScene(WorldEditorScene::Name);
		//	getEngine()->disableScene(MaterialEditorScene::Name);
		//	getEngine()->enableScene(AssetsEditorScene::Name, 1000);
		//}
		//ImGui::SameLine();
		//if (ImGui::Button("World Editor"))
		//{
		//	getEngine()->enableScene(WorldEditorScene::Name, 1000);
		//	getEngine()->disableScene(MaterialEditorScene::Name);
		//	getEngine()->disableScene(AssetsEditorScene::Name);
		//}
		//ImGui::SameLine();
		//if (ImGui::Button("Material Editor"))
		//{
		//	getEngine()->disableScene(WorldEditorScene::Name);
		//	getEngine()->enableScene(MaterialEditorScene::Name, 1000);
		//	getEngine()->disableScene(AssetsEditorScene::Name);
		//}
		//ImGui::EndChild();
		return true;
	}

	bool MainMenuScene::_userUpdateEnd(float time)
	{
		ImGui::End();
		//if (getInstance<Input>()->getPhysicalKeyJustReleased(AGE_ESCAPE))
		//	return (false);
		return true;
	}
}