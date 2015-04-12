#include "MaterialEditorScene.hh"
#include <imgui/imgui.h>
#include <Utils/OpenGL.hh>

namespace AGE
{
	const std::string MaterialEditorScene::Name = "MaterialEditor";

	MaterialEditorScene::MaterialEditorScene(AGE::Engine *engine)
		: AScene(engine)
	{
	}

	MaterialEditorScene::~MaterialEditorScene(void)
	{
	}

	bool MaterialEditorScene::_userStart()
	{
		return true;
	}

	bool MaterialEditorScene::_userUpdateBegin(float time)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::BeginChild("Material editor", ImVec2(0, 0), true);
		return true;
	}

	bool MaterialEditorScene::_userUpdateEnd(float time)
	{
		ImGui::EndChild();
		ImGui::End();
		return true;
	}
}