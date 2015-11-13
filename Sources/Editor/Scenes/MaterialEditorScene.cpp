#include "MaterialEditorScene.hh"
#include <imgui/imgui.h>
#include <Utils/OpenGL.hh>
#include <Convertor/MaterialConvertor.hpp>
#include <Utils/Directory.hpp>
#include <Utils/Path.hpp>
#include <FileUtils/FileSystemHelpers.hpp>
#include <EditorConfiguration.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "AssetsEditorScene.hpp"
#include "WorldEditorScene.hpp"
#include <AssetManagement/AssetManager.hh>

namespace AGE
{
	static const char *_nameTexture[ModeTexture::size] = {
		"ambient texture",
		"diffuse texture",
		"emissive texture",
		"reflective texture",
		"specular texture",
		"normal texture",
		"bump texture"
	};

	const std::string MaterialEditorScene::Name = "Material Editor";

	MaterialEditorScene::MaterialEditorScene(AGE::Engine *engine, std::shared_ptr<WorldEditorScene> const &world_editor)
		: IMenuInheritance(engine),
		_world_editor(world_editor),
		_mode(ModeMaterialEditor::selectMaterial),
		_indexMaterial(-1)
	{
		_resetEdition();
	}

	MaterialEditorScene::~MaterialEditorScene(void)
	{
	}

	void MaterialEditorScene::updateMenu()
	{

	}

	bool MaterialEditorScene::_userStart()
	{
		return true;
	}

	void MaterialEditorScene::_resetEdition()
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
			if (Directory::IsFile(*it) && FileUtils::GetExtension(*it) == "mage")
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
		ImGui::ListBox("List of material existing", &_indexMaterial, matListBox, static_cast<int>(materialFullPath.size()));
		delete[] matListBox;
		if (_indexMaterial != -1 && ImGui::Button("open a material"))
		{
			_resetEdition();

			std::shared_ptr<MaterialDataSet> material_data_set = std::make_shared<MaterialDataSet>();
			std::ifstream ifs(materialFullPath[_indexMaterial], std::ios::binary);
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

	void MaterialEditorScene::_editTexture(std::string &texturePath, ModeTexture mode, MaterialData &current)
	{
		if (current.selectedTextureIndex[mode] == 0
			&& texturePath.size() != 0)
		{
			int i = 1;
			auto slashRemoved = texturePath;
			slashRemoved.erase(std::remove_if(slashRemoved.begin(), slashRemoved.end(), [](char chr){ return chr == '/' || chr == '\\'; }), slashRemoved.end());
			for (auto &e : AssetsEditorScene::getCookedTextureListFullPath())
			{
				std::string copy = e;
				copy.erase(std::remove_if(copy.begin(), copy.end(), [](char chr){ return chr == '/' || chr == '\\'; }), copy.end());
				if (copy == slashRemoved)
				{
					current.selectedTextureIndex[mode] = i;
					break;
				}
				++i;
			}
		}

		if (ImGui::Combo(_nameTexture[mode], &current.selectedTextureIndex[mode], AssetsEditorScene::getCookedTextureListFullPath().data(), (int)AssetsEditorScene::getCookedTextureListFullPath().size()))
		{
			auto index = current.selectedTextureIndex[mode];
			texturePath = AssetsEditorScene::getCookedTextureListFullPath()[index];
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
		ImGui::ListBox("List of sub material", &_indexSubMaterial, matListBox, static_cast<int>(_current.collection.size()));
		delete[] matListBox;
		if (_indexSubMaterial != -1 && ImGui::Button("open a sub material"))
			_mode = ModeMaterialEditor::edit;
		if (_indexSubMaterial == -1)
			ImGui::Text("Please select one sub material for edition");
		if (_indexSubMaterial != -1 && ImGui::Button("Delete sub material"))
		{
			_current.collection[_indexSubMaterial] = _current.collection.back();
			_current.collection.pop_back();
		}
		if (ImGui::Button("Create sub material"))
		{
			_current.collection.resize(_current.collection.size() + 1);
		}
		_saveEdit();
	}

	void MaterialEditorScene::_saveEdit()
	{
		if (ImGui::Button("Save Edition")) {
			const std::string currentDir = Directory::GetCurrentDirectory();
			const std::string absPath = Path::AbsoluteName(currentDir.c_str(), WE::EditorConfiguration::GetCookedDirectory().c_str());
			std::ofstream file(absPath + _current.name + ".mage", std::ios::trunc | std::ios::binary);
			cereal::PortableBinaryOutputArchive ar(file);
			ar(_current);
			//_world_editor->getInstance<AssetsManager>()->requireMaterialReload(_current.name + ".mage");
			//_world_editor->getInstance<AssetsManager>()->reloadMaterial(_current.name + ".mage");
		}
	}

	void MaterialEditorScene::_editData()
	{
		if (ImGui::Button("precedent"))
			_mode = ModeMaterialEditor::selectSubMaterial;
		MaterialData &mat = _current.collection[_indexSubMaterial];
		ImGui::ColorEdit3("ambient", glm::value_ptr(mat.ambient));
		ImGui::ColorEdit3("diffuse", glm::value_ptr(mat.diffuse));
		ImGui::ColorEdit3("emissive", glm::value_ptr(mat.emissive));
		ImGui::ColorEdit3("reflective", glm::value_ptr(mat.reflective));
		ImGui::ColorEdit3("specular", glm::value_ptr(mat.specular));
		ImGui::SliderFloat("shininess", &mat.shininess, 0.0f, 1.0f);
		_editTexture(mat.diffuseTexPath, ModeTexture::diffuse, mat);
		_editTexture(mat.specularTexPath, ModeTexture::specular, mat);
		_editTexture(mat.ambientTexPath, ModeTexture::ambient, mat);
		_editTexture(mat.reflectiveTexPath, ModeTexture::reflective, mat);
		_editTexture(mat.emissiveTexPath, ModeTexture::emissive, mat);
		_editTexture(mat.normalTexPath, ModeTexture::normal, mat);
		_editTexture(mat.bumpTexPath, ModeTexture::bump, mat);
		ImGui::Checkbox("Scale UVs", &mat.scaleUVs);
		ImGui::SameLine(); ImGui::Text("Use it only for plane or boxes");
		_saveEdit();
	}

	bool MaterialEditorScene::_userUpdateBegin(float time)
	{
		if (_displayWindow == false)
		{
			return true;
		}
		ImGui::Begin("Material editor");
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
		ImGui::End();
		return true;
	}

	bool MaterialEditorScene::_userUpdateEnd(float time)
	{
		return true;
	}
}