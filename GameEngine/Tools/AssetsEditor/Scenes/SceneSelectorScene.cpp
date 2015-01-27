#include "SceneSelectorScene.hpp"
#include <imgui\imgui.h>

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
		std::vector<std::string> scenes;
		ImGui::Begin("Scene selector");
		getEngine().lock()->getSceneList(scenes);
		static int sceneIndex = 0;
		std::string sceneStr;
		for (auto i = 0; i < scenes.size(); ++i)
		{
			auto &e = scenes[i];
			if (e == Name)
			{
				std::swap(scenes.back(), e);
				scenes.pop_back();
				continue;
			}
			sceneStr += e;
			sceneStr += '\0';
		}
		if (ImGui::Combo("Select scene", &sceneIndex, sceneStr.c_str(), (int)(scenes.size())))
		{
			for (auto i = 0; i < scenes.size(); ++i)
			{
				if (i != sceneIndex)
					getEngine().lock()->disableScene(scenes[sceneIndex]);
			}
			getEngine().lock()->enableScene(scenes[sceneIndex], 1);
		}

		//const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK" };
		//static int item2 = -1;
		//ImGui::Combo("combo scroll", &item2, items, (int)());
		ImGui::End();
		return true;
	}
}