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
		AssetsEditorScene(AGE::Engine *engine);

		virtual ~AssetsEditorScene(void);
		virtual bool userStart();
		virtual bool userUpdateBegin(float time);
		virtual bool userUpdateEnd(float time);

		static AE::Folder &getRawList() { return _raw; }
		static AE::Folder &getCookedList() { return _cook; }

		static std::vector<const char *> &getCookedMeshsList() { return _cookedMeshFiles; }
		static std::vector<const char *> &getCookedMeshsListFullPath() { return _cookedMeshsFullPath; }
		static std::vector<const char *> &getCookedBulletList() { return _cookedBulletFiles; }
		static std::vector<const char *> &getCookedBulletListFullPath() { return _cookedBulletFullPath; }
		static std::vector<const char *> &getCookedMaterialList() { return _cookedMaterialFiles; }
		static std::vector<const char *> &getCookedMaterialListFullPath() { return _cookedMaterialFullPath; }
	private:
		static AE::Folder _raw;
	 	static AE::Folder _cook;

		struct AssetsEditorFileDescriptor
		{
			AssetsEditorFileDescriptor(const std::string &_fp, const std::string &_fn)
				: fullPath(_fp), fileName(_fn)
			{}
			std::string fullPath;
			std::string fileName;
		};

		static std::vector<AssetsEditorFileDescriptor> _cookedFiles;

		static std::vector<const char *> _cookedMeshsFullPath;
		static std::vector<const char *> _cookedMeshFiles;

		static std::vector<const char *> _cookedBulletFullPath;
		static std::vector<const char *> _cookedBulletFiles;

		static std::vector<const char *> _cookedMaterialFullPath;
		static std::vector<const char *> _cookedMaterialFiles;

		std::shared_ptr<AE::RawFile> _selectedRaw;
		std::set<std::shared_ptr<AE::CookConfig>> _configs;
	};
}