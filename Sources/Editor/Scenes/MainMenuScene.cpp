#include "MainMenuScene.hpp"
#include <imgui\imgui.h>
#include "AssetsEditorScene.hpp"
#include "WorldEditorScene.hpp"
#include "MaterialEditorScene.hh"
#include <Core/Inputs/Input.hh>
#include "IMenuInheritrance.hpp"

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
		getEngine()->enableScene(AGE::WorldEditorScene::Name, 10);
		getEngine()->enableScene(AGE::AssetsEditorScene::Name, 100);
		getEngine()->enableScene(AGE::MaterialEditorScene::Name, 1000);

		// we register tools names
		_toolsName[WorldEditor] = WorldEditorScene::Name;
		_toolsName[MaterialEditor] = MaterialEditorScene::Name;
		_toolsName[AssetsEditor] = AssetsEditorScene::Name;


		for (auto i = 0; i < TOOLS_NUMBER; ++i)
		{
			_scenes[i] = std::dynamic_pointer_cast<IMenuInheritance>(getEngine()->getScene(_toolsName[i])).get();
			AGE_ASSERT(_scenes[i] != nullptr);
		}

		_scenes[WorldEditor]->_displayWindow = true;

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
					bool notAlreadyActive = _scenes[i]->_displayWindow == false;

					if (ImGui::MenuItem(_toolsName[i].c_str(), nullptr, !notAlreadyActive))
					{
						_scenes[i]->_displayWindow = !_scenes[i]->_displayWindow;
						// hack
						if (ToolType(i) == WorldEditor)
						{
							if (_scenes[i]->_displayWindow)
							{
								getEngine()->enableScene(AGE::WorldEditorScene::Name, 10);
							}
							else
							{
								getEngine()->disableScene(AGE::WorldEditorScene::Name);
							}
						}
					}
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "CTRL+SHIFT+Q"))
				{
					_exitApp = true;
				}

				ImGui::EndMenu();
			}
			
			for (auto i = 0; i < TOOLS_NUMBER; ++i)
			{
				if (_scenes[i]->_displayWindow)
				{
					_scenes[i]->updateMenu();
				}
			}

			ImGui::EndMainMenuBar();
		}
		return true;
	}

	bool MainMenuScene::_userUpdateEnd(float time)
	{
		auto input = getInstance<Input>();
		
		auto ctrl = input->getPhysicalKeyPressed(AgeKeys::AGE_LCTRL);
		ctrl |= input->getPhysicalKeyPressed(AgeKeys::AGE_RCTRL);

		auto shift = input->getPhysicalKeyPressed(AgeKeys::AGE_LSHIFT);
		shift |= input->getPhysicalKeyPressed(AgeKeys::AGE_RSHIFT);

		auto qKey = input->getPhysicalKeyPressed(AgeKeys::AGE_q);

		if (ctrl && shift && qKey)
		{
			_exitApp = true;
		}
		return !_exitApp;
	}
}