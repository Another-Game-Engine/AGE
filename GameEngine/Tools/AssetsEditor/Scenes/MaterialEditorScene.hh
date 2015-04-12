#pragma once

#include <Core/AScene.hh>
#include <Core/Engine.hh>
#include <AssetManagement\Data\MaterialData.hh>
#include <memory>

#define NAME_LENGTH 50

namespace AGE
{
	enum class ModeMaterialEditor {
		menu,
		edit,
		size
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
		void _editName();
		void _editMaterial();
		void _menu();
		void _resetNameEdition();
	
	private:
		ModeMaterialEditor _mode;
		int _selectMaterial;
		MaterialDataSet _current;
		bool _editModeName;
		char _bufferName[NAME_LENGTH];
	};
}