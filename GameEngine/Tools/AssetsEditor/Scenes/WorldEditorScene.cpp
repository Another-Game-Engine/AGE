#include "WorldEditorScene.hpp"
#include <imgui\imgui.h>

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
		return true;
	}

	bool WorldEditorScene::userUpdate(double time)
	{
		ImGui::End();
		return true;
	}
}