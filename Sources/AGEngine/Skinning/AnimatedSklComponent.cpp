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
	{}

	void AnimatedSklComponent::init(const std::string &skeletonPath /*= ""*/, std::shared_ptr<Skeleton> skeletonAsset /*= nullptr*/)
	{
		if (skeletonAsset != nullptr)
			_skeletonAsset = skeletonAsset;
		else if (skeletonPath.empty() == false)
			_loadAndSetSkeleton(skeletonPath);
		_animIsShared = false;
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
	}

#ifdef EDITOR_ENABLED
	bool AnimatedSklComponent::editorUpdate()
	{ return true;}
#endif
}