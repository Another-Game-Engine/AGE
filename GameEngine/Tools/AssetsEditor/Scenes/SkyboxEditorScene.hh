#pragma once

#include <Core/AScene.hh>
#include <Core/Engine.hh>
#include <memory>
#include <AssetFiles/Folder.hpp>
#include <AssetFiles/RawFile.hpp>
#include <AssetManagement/Data/TextureData.hh>
#include <set>

#define NAME_LENGHT 50

namespace AGE
{

	class SkyboxEditorScene : public AScene
	{
	public:
		static const std::string Name;
		SkyboxEditorScene(AGE::Engine *engine);

		virtual ~SkyboxEditorScene(void);
		virtual bool _userStart();
		virtual bool _userUpdateBegin(float time);
		virtual bool _userUpdateEnd(float time);

	private:
		static AE::Folder _raw;
		std::shared_ptr<AE::RawFile> _selectedRaw;
		char name[NAME_LENGHT];
		int _current_skybox[SkyboxConfig::TOTAL_FACES];
	};
}