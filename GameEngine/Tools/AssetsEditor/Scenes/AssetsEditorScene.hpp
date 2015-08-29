#pragma once

#include <Core/AScene.hh>
#include <Core/Engine.hh>
#include <memory>
#include <FileUtils/AssetFiles/Folder.hpp>
#include <set>
#include "IMenuInheritrance.hpp"

namespace AGE
{
	namespace FileUtils
	{
		class RawFile;
	}
	namespace AE
	{
		struct CookConfig;
	}
	class AssetsEditorScene : public AScene, public IMenuInheritance
	{
	public:
		static const std::string Name;
		AssetsEditorScene(AGE::Engine *engine);

		virtual ~AssetsEditorScene(void);
		virtual bool _userStart();
		virtual bool _userUpdateBegin(float time);
		virtual bool _userUpdateEnd(float time);

		static FileUtils::Folder &getRawList() { return _raw; }
		static FileUtils::Folder &getCookedList() { return _cook; }

		static std::vector<const char *> &getCookedMeshsList() { return _cookedMeshFiles; }
		static std::vector<const char *> &getCookedMeshsListFullPath() { return _cookedMeshsFullPath; }
		static std::vector<const char *> &getCookedPhysicsList() { return _cookedPhysicsFiles; }
		static std::vector<const char *> &getCookedPhysicsListFullPath() { return _cookedPhysicsFullPath; }
		static std::vector<const char *> &getCookedMaterialList() { return _cookedMaterialFiles; }
		static std::vector<const char *> &getCookedMaterialListFullPath() { return _cookedMaterialFullPath; }
		static std::vector<const char *> &getCookedTextureList() { return _cookedTextureFiles; }
		static std::vector<const char *> &getCookedTextureListFullPath() { return _cookedTextureFullPath; }

		virtual void updateMenu();

	private:
		static FileUtils::Folder _raw;
	 	static FileUtils::Folder _cook;

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

		static std::vector<const char *> _cookedPhysicsFullPath;
		static std::vector<const char *> _cookedPhysicsFiles;

		static std::vector<const char *> _cookedMaterialFullPath;
		static std::vector<const char *> _cookedMaterialFiles;

		static std::vector<const char *> _cookedTextureFullPath;
		static std::vector<const char *> _cookedTextureFiles;

		std::shared_ptr<FileUtils::RawFile> _selectedRaw;
		std::set<std::shared_ptr<AE::CookConfig>> _configs;
	};
}