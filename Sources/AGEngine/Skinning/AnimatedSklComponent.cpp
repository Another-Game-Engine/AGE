#include "AnimatedSklComponent.hpp"

#include "Skeleton.hpp"
#include "AnimationManager.hpp"

#include "assetmanagement/assetmanager.hh"

#include "Entity/Entity.hh"
#include "Entity/EntityData.hh"

#include "Core/AScene.hh"

namespace AGE
{
	AnimatedSklComponent::AnimatedSklComponent()
		: _skeletonAsset(nullptr)
	{}

	AnimatedSklComponent::~AnimatedSklComponent(void)
	{}

	void AnimatedSklComponent::_copyFrom(const ComponentBase *model)
	{
#ifdef EDITOR_ENABLED
		editorCopyFrom((AnimatedSklComponent*)(model));
#endif
	}

	void AnimatedSklComponent::init(const std::string &skeletonPath /*= ""*/, std::shared_ptr<Skeleton> skeletonAsset /*= nullptr*/)
	{
		if (skeletonAsset != nullptr)
			_skeletonAsset = skeletonAsset;
		else if (skeletonPath.empty() == false)
			_loadAndSetSkeleton(skeletonPath);
		_animIsShared = false;
		_timeMultiplier = 10.0f;
#ifdef EDITOR_ENABLED
		editorCreate();
#endif
	}

	void AnimatedSklComponent::reset()
	{
		if (_animationInstance != nullptr)
		{
			auto animationManager = entity->getScene()->getInstance<AnimationManager>();
			AGE_ASSERT(animationManager != nullptr);
			animationManager->deleteAnimationInstance(_animationInstance);
		}
		_skeletonAsset = nullptr;
		_animationAsset = nullptr;
		_skeletonFilePath.clear();
		_animationFilePath.clear();
		_animationInstance = nullptr;
		_animIsShared = false;
#ifdef EDITOR_ENABLED
		editorDelete();
#endif
	}

	void AnimatedSklComponent::postUnserialization()
	{
		if (_skeletonFilePath.empty() == false)
			_loadAndSetSkeleton(_skeletonFilePath);
		if (_animationFilePath.empty() == false)
			_loadAndSetAnimation(_animationFilePath);
	}

	void AnimatedSklComponent::setAnimation(const std::string &animationPath, bool isShared /*= false*/)
	{
		_animIsShared = isShared;
		if (animationPath.empty() == false)
		{
			_loadAndSetAnimation(animationPath);
		}
	}

	void AnimatedSklComponent::setAnimation(std::shared_ptr<AnimationData> animationAssetPtr, bool isShared /*= false*/)
	{
		_animIsShared = isShared;
		AGE_ASSERT(animationAssetPtr != nullptr);
		_animationAsset = animationAssetPtr;
		_setAnimation();
	}

	void AnimatedSklComponent::_loadAndSetSkeleton(const std::string &skeletonPath)
	{
		AGE_ASSERT(skeletonPath.empty() == false);

		_skeletonFilePath = skeletonPath;
		auto assetsManager = entity->getScene()->getInstance<AssetsManager>();
		AGE_ASSERT(assetsManager != nullptr);
		_skeletonAsset = assetsManager->getSkeleton(skeletonPath);
		if (_skeletonAsset == nullptr)
		{
			assetsManager->pushNewCallback(skeletonPath, entity->getScene(),
				std::function<void()>([=](){
				this->_skeletonAsset = assetsManager->getSkeleton(skeletonPath);
				this->_setAnimation();
			}));
			assetsManager->loadSkeleton(skeletonPath, skeletonPath);
		}
	}

	void AnimatedSklComponent::_loadAndSetAnimation(const std::string &animationPath)
	{
		AGE_ASSERT(animationPath.empty() == false);

		_animationFilePath = animationPath;
		auto assetsManager = entity->getScene()->getInstance<AssetsManager>();
		AGE_ASSERT(assetsManager != nullptr);

		_animationAsset = assetsManager->getAnimation(animationPath);
		if (_animationAsset == nullptr)
		{
			assetsManager->pushNewCallback(animationPath, entity->getScene(),
				std::function<void()>([=](){
				this->_animationAsset = assetsManager->getAnimation(animationPath);
				this->_setAnimation();
			}));
			assetsManager->loadAnimation(animationPath, animationPath);
		}
		else
		{
			_setAnimation();
		}
	}

	void AnimatedSklComponent::_setAnimation()
	{
		if (_skeletonAsset == nullptr || _animationAsset == nullptr)
		{
			return;
		}
		auto animationManager = entity->getScene()->getInstance<AnimationManager>();
		AGE_ASSERT(animationManager != nullptr);
		if (_animationInstance != nullptr)
		{
			animationManager->deleteAnimationInstance(_animationInstance);
		}
		_animationInstance = animationManager->createAnimationInstance(_skeletonAsset, _animationAsset, _animIsShared);
		_animationInstance->_timeMultiplier = _timeMultiplier;
	}

}


// EDITOR
#ifdef EDITOR_ENABLED

#include <ImGui/ImGui.h>
#include <FileUtils/FileSystemHelpers.hpp>
#include <FileUtils/AssetFiles/CookedFileFilter.hpp>
#include <FileUtils/AssetFiles/Folder.hpp>
#include <FileUtils/AssetFiles/CookedFile.hpp>
#include "../Editor/EditorConfiguration.hpp"

namespace AGE
{
	std::string selectAnimation(const std::string &assetsDirectory)
	{
		static bool folderInitialized = false;
		static FileUtils::Folder meshFolder = FileUtils::Folder();

		bool hasChanged = false;

		if (folderInitialized == false)
		{
			FileUtils::CookedFileFilter filter;
			meshFolder.list(&filter, assetsDirectory);
			folderInitialized = true;
		}

		std::list<std::string> fbxInFolder;

		fbxInFolder.clear();

		meshFolder.update(
			std::function<bool(FileUtils::Folder*)>([&](FileUtils::Folder* folder) {
			return true;
		}),
			std::function<bool(FileUtils::Folder*)>([](FileUtils::Folder* folder) {
			return true;
		}),
			std::function<void(FileUtils::CookedFile*)>([&](FileUtils::CookedFile* file) {

			auto extension = FileUtils::GetExtension(file->getFileName());
			// Name of the file without extension and without _static or _dynamic
			std::string phageName;

			if (extension == "aage")
			{
				phageName = file->getPath();
				auto find = phageName.find(assetsDirectory);
				if (find != std::string::npos)
				{
					phageName.erase(find, find + assetsDirectory.size());
				}
				fbxInFolder.push_back(phageName);
			}
		}));

		std::string res = "";
		bool clicked = false;

		if (ImGui::BeginPopupModal("Select Animation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static ImGuiTextFilter filter;

			filter.Draw();
			for (auto &e : fbxInFolder)
			{
				if (filter.PassFilter(e.c_str()))
				{
					if (ImGui::SmallButton(e.c_str()))
					{
						res = e;
						clicked = true;
						ImGui::CloseCurrentPopup();
					}
				}
			}
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { res = "NULL"; ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}
		return res;
	}

	std::string selectSkeleton(const std::string &assetsDirectory)
	{
		static bool folderInitialized = false;
		static FileUtils::Folder meshFolder = FileUtils::Folder();

		bool hasChanged = false;

		if (folderInitialized == false)
		{
			FileUtils::CookedFileFilter filter;
			meshFolder.list(&filter, assetsDirectory);
			folderInitialized = true;
		}

		std::list<std::string> fbxInFolder;

		fbxInFolder.clear();

		meshFolder.update(
			std::function<bool(FileUtils::Folder*)>([&](FileUtils::Folder* folder) {
			return true;
		}),
			std::function<bool(FileUtils::Folder*)>([](FileUtils::Folder* folder) {
			return true;
		}),
			std::function<void(FileUtils::CookedFile*)>([&](FileUtils::CookedFile* file) {

			auto extension = FileUtils::GetExtension(file->getFileName());
			// Name of the file without extension and without _static or _dynamic
			std::string phageName;

			if (extension == "skage")
			{
				phageName = file->getPath();
				auto find = phageName.find(assetsDirectory);
				if (find != std::string::npos)
				{
					phageName.erase(find, find + assetsDirectory.size());
				}
				fbxInFolder.push_back(phageName);
			}
		}));

		std::string res = "";
		bool clicked = false;

		if (ImGui::BeginPopupModal("Select Skeleton", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static ImGuiTextFilter filter;

			filter.Draw();
			for (auto &e : fbxInFolder)
			{
				if (filter.PassFilter(e.c_str()))
				{
					if (ImGui::SmallButton(e.c_str()))
					{
						res = e;
						clicked = true;
						ImGui::CloseCurrentPopup();
					}
				}
			}
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { res = "NULL"; ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}
		return res;
	}


	std::shared_ptr<AnimatedSklComponent::Config> AnimatedSklComponent::getSharedConfig()
	{
		if (_config->isShared == true)
		{
			for (auto it = std::begin(getSharedConfigList()); it != std::end(getSharedConfigList()); ++it)
			{
				if ((*it)->skeletonFilePath == _config->skeletonFilePath
					&& (*it)->animationFilePath == _config->animationFilePath)
				{
					return *it;
				}
			}
		}
		return nullptr;
	}

	void AnimatedSklComponent::cleanConfigPtr()
	{
		Config *ptr = _config.get();
		_config = nullptr;

		if (ptr->isShared == true)
		{
			for (auto it = std::begin(getSharedConfigList()); it != std::end(getSharedConfigList()); ++it)
			{
				if ((*it).get() == ptr)
				{
					if ((*it).use_count() == 1)
					{
						getSharedConfigList().erase(it);
					}
					break;
				}
			}
		}
	}

	void AnimatedSklComponent::editorCopyFrom(AnimatedSklComponent *o)
	{
		if (_config == nullptr)
			_config = std::make_shared<Config>();

		if (o->_config->isShared)
			_config = o->_config;
		else
		{
			_config->animationFilePath = o->_config->animationFilePath;
			_config->skeletonFilePath = o->_config->skeletonFilePath;
			_config->timeMultiplier = o->_config->timeMultiplier;
		}
	}

	void AnimatedSklComponent::editorCreate()
	{
		if (!_config)
			_config = std::make_shared<Config>();
	}
	void AnimatedSklComponent::editorDelete()
	{
		if (_config)
			return;
	}

	bool AnimatedSklComponent::editorUpdate()
	{
		bool modified = false;

		static std::string *selectedSkeleton = nullptr;
		static std::string *selectedAnimation = nullptr;

		if (selectedSkeleton != nullptr)
		{
			*selectedSkeleton = selectSkeleton(entity->getScene()->getInstance<AGE::AssetsManager>()->getAssetsDirectory());
			if (*selectedSkeleton == "NULL")
			{
				*selectedSkeleton = "";
				selectedSkeleton = nullptr;
			}
			else if (selectedSkeleton->empty() == false)
			{
				selectedSkeleton = nullptr;
			}
			return true;
		}

		if (selectedAnimation != nullptr)
		{
			*selectedAnimation = selectAnimation(entity->getScene()->getInstance<AGE::AssetsManager>()->getAssetsDirectory());
			if (*selectedAnimation == "NULL")
			{
				*selectedAnimation = "";
				selectedAnimation = nullptr;
			}
			else if (selectedAnimation->empty() == false)
			{
				selectedAnimation = nullptr;
			}
			return true;
		}

		/////////////////////////////////

		if (_config->skeletonFilePath.empty())
		{
			if (ImGui::Button("Select skeleton"))
			{
				ImGui::OpenPopup("Select Skeleton");
				selectedSkeleton= &_config->skeletonFilePath;
			}
		}
		else
		{
			ImGui::Text(_config->skeletonFilePath.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Edit skeleton"))
			{
				ImGui::OpenPopup("Select Skeleton");
				selectedSkeleton = &_config->skeletonFilePath;
			}
		}

		if (_config->animationFilePath.empty())
		{
			if (ImGui::Button("Select animation"))
			{
				ImGui::OpenPopup("Select Animation");
				selectedAnimation = &_config->animationFilePath;
			}
		}
		else
		{
			ImGui::Text(_config->animationFilePath.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Edit animation"))
			{
				ImGui::OpenPopup("Select Animation");
				selectedAnimation = &_config->animationFilePath;
			}
		}

		modified |= ImGui::InputFloat("Speed", &_config->timeMultiplier);

		if (ImGui::Checkbox("Is shared", &_config->isShared))
		{
			modified = true;
			if (_config->isShared)
			{
				auto ptr = getSharedConfig();
				if (ptr == nullptr)
				{
					getSharedConfigList().push_back(_config);
				}
				else
				{
					_config = ptr;
				}
			}
			else
			{
				_config->isShared = true;
				auto ptr = std::make_shared<Config>(*_config.get());
				ptr->isShared = false;
				cleanConfigPtr();
				_config = ptr;
			}
		}

		return modified;
	}
}
#endif
