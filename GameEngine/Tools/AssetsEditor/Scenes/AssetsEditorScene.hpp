#pragma once

#include <Core/AScene.hh>
#include <Core/Engine.hh>
#include <memory>
#include <AssetFiles/Folder.hpp>

namespace AGE
{
	namespace AE
	{
		class RawFile;
	}
	class AssetsEditorScene : public AScene
	{
	public:
		AssetsEditorScene(std::weak_ptr<AGE::Engine> engine);

		virtual ~AssetsEditorScene(void);
		virtual bool userStart();
		virtual bool userUpdate(double time);

	private:
		AE::Folder _raw;
	 	AE::Folder _cook;
		std::set<std::shared_ptr<AE::RawFile>> _selectedRaw;
	};
}