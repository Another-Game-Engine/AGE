#pragma once

#include <Components/Component.hh>

namespace AGE
{
	struct Skeleton;
	struct AnimationData;
	struct AnimationInstance;

	class AnimatedSklComponent : public ComponentBase
	{
		AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_SKINNING_AnimatedSklComponent");

		AnimatedSklComponent();
		virtual ~AnimatedSklComponent(void);
		AnimatedSklComponent &operator=(const AnimatedSklComponent &o) = delete;
		virtual void _copyFrom(const ComponentBase *model);

		void init(const std::string &skeletonPath = "", std::shared_ptr<Skeleton> skeletonAsset = nullptr);
		void setAnimation(const std::string &animationPath);
		void setAnimation(std::shared_ptr<AnimationData> animationAssetPtr);

		virtual void reset();

		inline std::shared_ptr<Skeleton> getSkeleton() { return _skeletonAsset; }

		//////
		////
		// Serialization

		// Load Json
		// Save Json
		// Editor only
		template <class Archive, cereal::traits::EnableIf<cereal::traits::is_text_archive<Archive>::value> = cereal::traits::sfinae>
		void serialize(Archive &ar, const std::uint32_t version)
		{
			ar(CEREAL_NVP(_skeletonFilePath));
		}

		// Load Binary
		// Save Binary
		// In game
		template <class Archive, cereal::traits::DisableIf<cereal::traits::is_text_archive<Archive>::value> = cereal::traits::sfinae>
		void save(Archive ar, const std::uint32_t version) const
		{
			ar(std::string(_skeletonAsset->path));
		}

		template <class Archive, cereal::traits::DisableIf<cereal::traits::is_text_archive<Archive>::value> = cereal::traits::sfinae>
		void load(Archive &ar, const std::uint32_t version)
		{
			ar(_skeletonFilePath);
		}

		// !Serialization
		////
		//////

#ifdef EDITOR_ENABLED
		virtual bool editorUpdate();
#endif
	private:
		std::string _skeletonFilePath;
		std::shared_ptr<Skeleton> _skeletonAsset;

		std::string _animationFilePath;
		std::shared_ptr<AnimationData> _animationAsset;

		std::shared_ptr<AnimationInstance> _animationInstance;

		void _loadAndSetSkeleton(const std::string &skeletonPath);
		void _loadAndSetAnimation(const std::string &animationPath);
		void _setAnimation();
	};
}

CEREAL_CLASS_VERSION(AGE::AnimatedSklComponent, 0)