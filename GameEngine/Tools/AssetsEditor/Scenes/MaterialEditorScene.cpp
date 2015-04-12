#include "MaterialEditorScene.hh"
#include <imgui/imgui.h>
#include <Utils/OpenGL.hh>
#include <Convertor/MaterialConvertor.hpp>
#include <Utils/Directory.hpp>
#include <Utils/Path.hpp>
#include <Utils/FileSystemHelpers.hpp>
#include <EditorConfiguration.hpp>

namespace AGE
{
	const std::string MaterialEditorScene::Name = "MaterialEditor";

	MaterialEditorScene::MaterialEditorScene(AGE::Engine *engine)
		: AScene(engine),
		_mode(ModeMaterialEditor::menu),
		_selectMaterial(-1),
		_editModeName(false)
	{
		memset(_bufferName, 0, NAME_LENGTH);
	}

	MaterialEditorScene::~MaterialEditorScene(void)
	{
	}

	bool MaterialEditorScene::_userStart()
	{
		return true;
	}

	void MaterialEditorScene::_resetNameEdition()
	{
		_editModeName = false;
		memset(_bufferName, 0, NAME_LENGTH);
	}

	void MaterialEditorScene::_menu()
	{
		const std::string currentDir = Directory::GetCurrentDirectory();
		const std::string absPath = Path::AbsoluteName(currentDir.c_str(), WE::EditorConfiguration::GetCookedDirectory().c_str());
		std::vector<std::string> _materialGettable;
		std::vector<std::string> _materialFullPath;
		Directory dir;
		const bool succeed = dir.open(absPath.c_str());
		AGE_ASSERT(succeed && "Impossible to open directory");
		for (auto it = dir.recursive_begin(); it != dir.recursive_end(); ++it)
		{
			if (Directory::IsFile(*it) && AGE::FileSystemHelpers::GetExtension(*it) == "mage")
			{
				_materialGettable.push_back(std::string(Path::RelativeName(absPath.c_str(), *it)));
				_materialFullPath.push_back(*it);
			}
		}
		dir.close();
		char const **matListBox = new char const *[_materialGettable.size()];
		for (auto index = 0; index < _materialGettable.size(); ++index) {
			matListBox[index] = _materialGettable[index].c_str();
		}
		ImGui::ListBox("List of Material existing", &_selectMaterial, matListBox, _materialGettable.size());
		delete[] matListBox;
		if (_selectMaterial != -1 && ImGui::Button("open a material"))
		{
			_resetNameEdition();
			std::shared_ptr<MaterialDataSet> material_data_set = std::make_shared<MaterialDataSet>();
			std::ifstream ifs(_materialFullPath[_selectMaterial]);
			cereal::PortableBinaryInputArchive ar(ifs);
			ar(*material_data_set.get());
			_current = *material_data_set;
			if (_current.name == "")
			{
				std::string fileName = std::string(_materialGettable[_selectMaterial]);
				_current.name = fileName.substr(0, fileName.find('.'));
			}
			_mode = ModeMaterialEditor::edit;
		}
		if (ImGui::Button("create a new material"))
		{
			_resetNameEdition();
			_current = MaterialDataSet();
			_mode = ModeMaterialEditor::edit;
		}
	}

	void MaterialEditorScene::_editName()
	{
		if (!_editModeName)
		{
			if (_current.name.empty())
				ImGui::Text("No name...");
			else
				ImGui::Text(_current.name.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Edit name"))
				_editModeName = true;
		}
		else
		{
			ImGui::InputText("", _bufferName, NAME_LENGTH);
			ImGui::SameLine();
			if (ImGui::Button("valid new name"))
			{
				_current.name = std::string(_bufferName);
				_editModeName = false;
			}
		}
	}

	void MaterialEditorScene::_editMaterial()
	{
		if (ImGui::Button("precedent"))
			_mode = ModeMaterialEditor::menu;
		ImGui::Separator();
		ImGui::Spacing();
		_editName();
		if (ImGui::Button("cook")) {
			std::string const currentDir = Directory::GetCurrentDirectory();
			const std::string absPath = Path::AbsoluteName(currentDir.c_str(), WE::EditorConfiguration::GetCookedDirectory().c_str());
			auto fileName = absPath + _current.name + ".mage";
		}
	}

	bool MaterialEditorScene::_userUpdateBegin(float time)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::BeginChild("Material editor", ImVec2(0, 0), true);
		switch (_mode) {
		case ModeMaterialEditor::menu:
			_menu();
			break;
		case ModeMaterialEditor::edit:
			_editMaterial();
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