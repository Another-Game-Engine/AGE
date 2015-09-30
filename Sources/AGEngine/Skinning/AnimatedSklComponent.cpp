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

	void AnimatedSklComponent::init(const std::string &skeletonPath /*= ""*/, std::shared_ptr<Skeleton> skeletonAsset /*= nullptr*/)
	{
		if (skeletonAsset != nullptr)
			_skeletonAsset = skeletonAsset;
		else if (skeletonPath.empty() == false)
			_loadAndSetSkeleton(skeletonPath);
	}

	void AnimatedSklComponent::reset()
	{
		_skeletonAsset = nullptr;
		_skeletonFilePath.clear();
	}

	void AnimatedSklComponent::_loadAndSetSkeleton(const std::string &skeletonPath)
	{
		_skeletonFilePath = skeletonPath;
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
	{ return true;}
#endif
}