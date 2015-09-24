#pragma once

#include <AssetManagement\Data\MaterialData.hh>
#include <memory>
#include "IMenuInheritrance.hpp"

#define NAME_LENGTH 50
#define TEXTURE_LENGTH 255

#define InputTextCustom(name, diff1, diff2) memcpy(diff1, mat.diff2.c_str(), mat.diff2.size()); \
	ImGui::InputText(name, diff1, mat.diff2.size());

namespace AGE { class WorldEditorScene; }

namespace AGE
{
	class Engine;

	enum class ModeMaterialEditor {
		selectMaterial,
		selectSubMaterial,
		edit
	};

	enum ModeTexture
	{
		ambient = 0,
		diffuse,
		emissive,
		reflective,
		specular,
		normal,
		bump,
		size
	};

	class MaterialEditorScene : public IMenuInheritance
	{
	public:
		static const std::string Name;

		MaterialEditorScene(Engine *engine, std::shared_ptr<WorldEditorScene> const &world_editor);

		virtual void updateMenu();

		virtual ~MaterialEditorScene(void);
		virtual bool _userStart();
		virtual bool _userUpdateBegin(float time);
		virtual bool _userUpdateEnd(float time);
		void _selectMaterial();
		void _selectSubMaterial();
		void _editName();
		void _editTexture(std::string &texturePath, ModeTexture mode, MaterialData &current);
		void _editData();
		void _saveEdit();
		void _resetEdition();
	
	private:
		std::shared_ptr<WorldEditorScene> _world_editor;
		ModeMaterialEditor _mode;
		int _indexMaterial;
		int _indexSubMaterial;
		MaterialDataSet _current;
		bool _editModeName;
		char _bufferName[NAME_LENGTH];
	};
}