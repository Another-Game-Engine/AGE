#pragma once

#include <Components/Component.hh>

namespace AGE
{
	struct Skeleton;

	class AnimatedSklComponent : public ComponentBase
	{
		AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_SKINNING_AnimatedSklComponent");

		AnimatedSklComponent();
		virtual ~AnimatedSklComponent(void);
		AnimatedSklComponent &operator=(const AnimatedSklComponent &o) = delete;
		virtual void _copyFrom(const ComponentBase *model);

		void init(const std::string &skeletonPath = "");
		void init(std::shared_ptr<Skeleton> skeletonAsset = nullptr);

		virtual void reset();

		//////
		////
		// Serialization

		template <typename Archive>
		void serialize(Archive &ar, uint32_t version)
		{
		}

		// !Serialization
		////
		//////

#ifdef EDITOR_ENABLED
		virtual bool editorUpdate();
	private:
		std::string _skeletonFilePath;
#endif
	private:
		std::shared_ptr<Skeleton> _skeletonAsset;

		void _loadAndSetSkeleton(const std::string &skeletonPath);
	};
}