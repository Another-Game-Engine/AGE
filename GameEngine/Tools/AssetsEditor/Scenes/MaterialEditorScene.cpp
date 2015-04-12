#include "MaterialEditorScene.hh"
#include <imgui/imgui.h>
#include <Utils/OpenGL.hh>
#include <AssetManagement\Data\MaterialData.hh>
#include <Convertor/MaterialConvertor.hpp>
#include <Utils/Directory.hpp>
#include <Utils/Path.hpp>
#include <EditorConfiguration.hpp>

#define NAME_LENGTH 50

namespace AGE
{
	const std::string MaterialEditorScene::Name = "MaterialEditor";

	MaterialEditorScene::MaterialEditorScene(AGE::Engine *engine)
		: AScene(engine),
		_mode(ModeMaterialEditor::main)
	{
	}

	MaterialEditorScene::~MaterialEditorScene(void)
	{
	}

	bool MaterialEditorScene::_userStart()
	{
		return true;
	}

	void MaterialEditorScene::_main()
	{
		if (ImGui::Button("create a new material"))
			_mode = ModeMaterialEditor::creation;
	}

	void MaterialEditorScene::_createMaterial()
	{
		if (ImGui::Button("precedent"))
			_mode = ModeMaterialEditor::main;
		ImGui::Separator();
		ImGui::Spacing();
		char buffer[NAME_LENGTH];
		memset(buffer, 0, NAME_LENGTH);
		auto materialSet = std::make_shared<MaterialDataSet>();
		ImGui::InputText("Material Name: ", buffer, NAME_LENGTH);
		std::string name(buffer);
		if (ImGui::Button("cook")) {
			std::string const currentDir = Directory::GetCurrentDirectory();
			const std::string absPath = Path::AbsoluteName(currentDir.c_str(), WE::EditorConfiguration::GetCookedDirectory().c_str());
			auto fileName = absPath + name + ".mage";
		}
	}

	bool MaterialEditorScene::_userUpdateBegin(float time)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::BeginChild("Material editor", ImVec2(0, 0), true);
		switch (_mode) {
		case ModeMaterialEditor::main:
			_main();
			break;
		case ModeMaterialEditor::creation:
			_createMaterial();
			break;
		}
		return true;
	}

	bool MaterialEditorScene::_userUpdateEnd(float time)
	{
		ImGui::EndChild();
		ImGui::End();
		return true;
	}
}