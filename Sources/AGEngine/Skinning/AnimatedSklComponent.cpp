#include "AnimatedSklComponent.hpp"
#include "Skeleton.hpp"

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

	void AnimatedSklComponent::init(const std::string &skeletonPath /*= ""*/)
	{
		_loadAndSetSkeleton(skeletonPath);
	}

	void AnimatedSklComponent::init(std::shared_ptr<Skeleton> skeletonAsset /*= nullptr*/)
	{}

	void AnimatedSklComponent::reset()
	{
		_skeletonAsset = nullptr;
#if defined(EDITOR_ENABLED)
		_skeletonFilePath.clear();
#endif
	}

	void AnimatedSklComponent::_loadAndSetSkeleton(const std::string &skeletonPath)
	{
#if defined(EDITOR_ENABLED)
		_skeletonFilePath = skeletonPath;
#endif
		auto assetsManager = entity->getScene()->getInstance<AssetsManager>();
		AGE_ASSERT(assetsManager != nullptr);
		_skeletonAsset = assetsManager->getSkeleton(skeletonPath);
		if (!skeletonPath.empty() && (_skeletonAsset == nullptr))
		{
			assetsManager->pushNewCallback(skeletonPath, entity->getScene(),
				std::function<void()>([=](){
				_skeletonAsset = assetsManager->getSkeleton(skeletonPath);
			}));
			assetsManager->loadSkeleton(skeletonPath);
		}
	}

#ifdef EDITOR_ENABLED
	bool AnimatedSklComponent::editorUpdate()
	{
		return true;
	}
#endif
}