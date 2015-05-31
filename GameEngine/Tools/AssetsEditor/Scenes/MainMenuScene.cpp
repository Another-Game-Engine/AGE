#include "MainMenuScene.hpp"
#include <imgui\imgui.h>
#include "AssetsEditorScene.hpp"
#include "WorldEditorScene.hpp"
#include "MaterialEditorScene.hh"
#include <Core/Inputs/Input.hh>


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
		getEngine()->enableScene(AGE::WorldEditorScene::Name, 1000);

		// we register tools names
		_toolsName[WorldEditor] = WorldEditorScene::Name;
		_toolsName[MaterialEditor] = MaterialEditorScene::Name;
		_toolsName[AssetsEditor] = AssetsEditorScene::Name;

		_exitApp = false;
		
		return true;
	}

	bool MainMenuScene::_userUpdateBegin(float time)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Tools"))
			{
				for (auto i = 0; i < TOOLS_NUMBER; ++i)
				{
					bool notAlreadyActive = ToolType(i) != _selectedTool;

					if (ImGui::MenuItem(_toolsName[i].c_str(), nullptr, nullptr, notAlreadyActive))
					{
						getEngine()->disableScene(_toolsName[_selectedTool]);
						_selectedTool = ToolType(i);
						getEngine()->enableScene(_toolsName[_selectedTool], 1000);
					}
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "CTRL+SHIFT+Q"))
				{
					_exitApp = true;
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
		return true;
	}

	bool MainMenuScene::_userUpdateEnd(float time)
	{
		auto input = getInstance<Input>();
		
		auto ctrl = input->getPhysicalKeyJustReleased(AGE_LCTRL);
		ctrl |= input->getPhysicalKeyJustReleased(AGE_RCTRL);

		auto shift = input->getPhysicalKeyJustReleased(AGE_LSHIFT);
		shift |= input->getPhysicalKeyJustReleased(AGE_RSHIFT);

		auto qKey = input->getPhysicalKeyJustReleased(AGE_q);

		if (ctrl + shift + qKey)
		{
			_exitApp = true;
		}
		return !_exitApp;
	}
}