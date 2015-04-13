#include "MaterialEditorScene.hh"
#include <imgui/imgui.h>
#include <Utils/OpenGL.hh>
#include <Convertor/MaterialConvertor.hpp>
#include <Utils/Directory.hpp>
#include <Utils/Path.hpp>
#include <Utils/FileSystemHelpers.hpp>
#include <EditorConfiguration.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace AGE
{
	const std::string MaterialEditorScene::Name = "MaterialEditor";

	MaterialEditorScene::MaterialEditorScene(AGE::Engine *engine)
		: AScene(engine),
		_mode(ModeMaterialEditor::selectMaterial),
		_indexMaterial(-1),
		_indexSubMaterial(-1),
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
		_indexSubMaterial = -1;
		memset(_bufferName, 0, NAME_LENGTH);
	}

	void MaterialEditorScene::_selectMaterial()
	{
		const std::string currentDir = Directory::GetCurrentDirectory();
		const std::string absPath = Path::AbsoluteName(currentDir.c_str(), WE::EditorConfiguration::GetCookedDirectory().c_str());
		std::vector<std::string> materialGettable;
		std::vector<std::string> materialFullPath;
		Directory dir;
		const bool succeed = dir.open(absPath.c_str());
		AGE_ASSERT(succeed && "Impossible to open directory");
		for (auto it = dir.recursive_begin(); it != dir.recursive_end(); ++it)
		{
			if (Directory::IsFile(*it) && AGE::FileSystemHelpers::GetExtension(*it) == "mage")
			{
				materialGettable.push_back(std::string(Path::RelativeName(absPath.c_str(), *it)));
				materialFullPath.push_back(*it);
			}
		}
		dir.close();
		char const **matListBox = new char const *[materialGettable.size()];
		for (auto index = 0; index < materialFullPath.size(); ++index) {
			matListBox[index] = materialGettable[index].c_str();
		}
		ImGui::ListBox("List of material existing", &_indexMaterial, matListBox, materialFullPath.size());
		delete[] matListBox;
		if (_indexMaterial != -1 && ImGui::Button("open a material"))
		{
			_resetNameEdition();
			std::shared_ptr<MaterialDataSet> material_data_set = std::make_shared<MaterialDataSet>();
			std::ifstream ifs(materialFullPath[_indexMaterial]);
			cereal::PortableBinaryInputArchive ar(ifs);
			ar(*material_data_set.get());
			_current = *material_data_set;
			if (_current.name == "")
			{
				std::string fileName = std::string(materialGettable[_indexMaterial]);
				_current.name = fileName.substr(0, fileName.find('.'));
			}
			_mode = ModeMaterialEditor::selectSubMaterial;
		}
		if (_indexMaterial == -1)
		{
			ImGui::Text("Please select one material for edition");
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

	void MaterialEditorScene::_selectSubMaterial()
	{
		if (ImGui::Button("precedent"))
			_mode = ModeMaterialEditor::selectMaterial;
		ImGui::Separator();
		ImGui::Spacing();
		_editName();
		std::vector<std::string> subMaterials;
		for (auto index = 0; index < _current.collection.size(); ++index)
			subMaterials.emplace_back(std::string("sub material: ") + std::to_string(index));
		char const **matListBox = new char const *[_current.collection.size()];
		for (auto index = 0; index < _current.collection.size(); ++index)
			matListBox[index] = subMaterials[index].c_str();
		ImGui::ListBox("List of sub material", &_indexSubMaterial, matListBox, _current.collection.size());
		delete[] matListBox;
		if (_indexSubMaterial != -1 && ImGui::Button("open a sub material"))
			_mode = ModeMaterialEditor::edit;
		if (_indexSubMaterial == -1)
			ImGui::Text("Please select one sub material for edition");
		_saveEdit();
	}

	void MaterialEditorScene::_saveEdit()
	{
		if (ImGui::Button("Save Edition")) {
			const std::string currentDir = Directory::GetCurrentDirectory();
			const std::string absPath = Path::AbsoluteName(currentDir.c_str(), WE::EditorConfiguration::GetCookedDirectory().c_str());
			std::ofstream file(absPath + _current.name + ".mage");
			cereal::PortableBinaryOutputArchive ar(file);
			ar(_current);
		}
	}

	void MaterialEditorScene::_editData()
	{
		if (ImGui::Button("precedent"))
			_mode = ModeMaterialEditor::selectSubMaterial;
		MaterialData &mat = _current.collection[_indexSubMaterial];
		ImGui::InputFloat3("ambient", glm::value_ptr(mat.ambient));
		ImGui::InputFloat3("diffuse", glm::value_ptr(mat.diffuse));
		ImGui::InputFloat3("emissive", glm::value_ptr(mat.emissive));
		ImGui::InputFloat3("reflective", glm::value_ptr(mat.reflective));
		ImGui::InputFloat3("specular", glm::value_ptr(mat.specular));
		_saveEdit();
	}

	bool MaterialEditorScene::_userUpdateBegin(float time)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::BeginChild("Material editor", ImVec2(0, 0), true);
		switch (_mode) {
		case ModeMaterialEditor::selectMaterial:
			_selectMaterial();
			break;
		case ModeMaterialEditor::selectSubMaterial:
			_selectSubMaterial();
			break;
		case ModeMaterialEditor::edit:
			_editData();
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