#pragma once

#include <Core/AScene.hh>
#include <Core/Engine.hh>
#include <memory>
#include <AssetFiles/Folder.hpp>
#include <set>

namespace AGE
{
	namespace AE
	{
		class RawFile;
		class CookConfig;
	}
	class AssetsEditorScene : public AScene
	{
	public:
		static const std::string Name;
		AssetsEditorScene(std::weak_ptr<AGE::Engine> engine);

		virtual ~AssetsEditorScene(void);
		virtual bool userStart();
		virtual bool userUpdateBegin(double time);
		virtual bool userUpdateEnd(double time);

		static AE::Folder &getRawList() { return _raw; }
		static AE::Folder &getCookedList() { return _cook; }

	private:
		static AE::Folder _raw;
	 	static AE::Folder _cook;
		std::shared_ptr<AE::RawFile> _selectedRaw;
		std::set<std::shared_ptr<AE::CookConfig>> _configs;
	};
}