#pragma once

#include <Core/AScene.hh>
#include <Core/Engine.hh>
#include <AssetManagement\Data\MaterialData.hh>
#include <memory>

#define NAME_LENGTH 50

namespace AGE
{
	enum class ModeMaterialEditor {
		selectMaterial,
		selectSubMaterial,
		edit
	};

	class MaterialEditorScene : public AScene
	{
	public:
		static const std::string Name;

		MaterialEditorScene(Engine *engine);

		virtual ~MaterialEditorScene(void);
		virtual bool _userStart();
		virtual bool _userUpdateBegin(float time);
		virtual bool _userUpdateEnd(float time);
		void _selectMaterial();
		void _selectSubMaterial();
		void _editName();
		void _editData();
		void _saveEdit();




		void _resetNameEdition();
	
	private:
		ModeMaterialEditor _mode;
		int _indexMaterial;
		int _indexSubMaterial;
		MaterialDataSet _current;
		bool _editModeName;
		char _bufferName[NAME_LENGTH];
	};
}