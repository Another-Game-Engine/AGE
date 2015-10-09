#pragma once

#include <Components/Component.hh>

#ifdef EDITOR_ENABLED
#include <list>
#endif

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
		void setAnimation(const std::string &animationPath, bool isShared = false);
		void setAnimation(std::shared_ptr<AnimationData> animationAssetPtr, bool isShared = false);

		virtual void reset();
		virtual void postUnserialization();

		inline std::shared_ptr<Skeleton> getSkeleton() { return _skeletonAsset; }
		inline std::shared_ptr<AnimationData> getAnimationData() { return _animationAsset; }
		inline std::shared_ptr<AnimationInstance> getAnimation() { return _animationInstance; }
	private:
		std::string _skeletonFilePath;
		std::shared_ptr<Skeleton> _skeletonAsset;

		std::string _animationFilePath;
		std::shared_ptr<AnimationData> _animationAsset;

		std::shared_ptr<AnimationInstance> _animationInstance;

		float _timeMultiplier;

		bool _animIsShared;

#ifdef EDITOR_ENABLED
		struct Config
		{
			float timeMultiplier = 10.0f;
			bool  isShared = false;
			std::string skeletonFilePath;
			std::string animationFilePath;
		};

		std::shared_ptr<Config> getSharedConfig();
		void cleanConfigPtr();
		void editorCopyFrom(AnimatedSklComponent *o);
		void editorCreate();
		void editorDelete();
		virtual bool editorUpdate();
		
		std::shared_ptr<Config> _config;
		inline std::list<std::shared_ptr<Config>> &getSharedConfigList()
		{
			//not thread safe but shouldn't be called by others that main thread
			static std::list<std::shared_ptr<Config>> config;
			return config;
		}
#endif


		void _loadAndSetSkeleton(const std::string &skeletonPath);
		void _loadAndSetAnimation(const std::string &animationPath);
		void _setAnimation();





	public:
		//////
		////
		// Serialization

		// Load Json
		// Save Json
		// Editor only
		template <class Archive, cereal::traits::EnableIf<cereal::traits::is_text_archive<Archive>::value> = cereal::traits::sfinae>
		void serialize(Archive &ar, const std::uint32_t version)
		{
#if defined(EDITOR_ENABLED)
			if (_config == nullptr)
			{
				_config = std::make_shared<Config>();
			}
			ar(cereal::make_nvp("Skeleton", _config->skeletonFilePath));
			ar(cereal::make_nvp("Animation", _config->animationFilePath));
			ar(cereal::make_nvp("IsShared", _config->isShared));
			ar(cereal::make_nvp("TimeMultiplier", _config->timeMultiplier));
#endif
		}

		// Load Binary
		// Save Binary
		// In game
		template <class Archive, cereal::traits::DisableIf<cereal::traits::is_text_archive<Archive>::value> = cereal::traits::sfinae>
		void save(Archive ar, const std::uint32_t version) const
		{
			std::string skeletonFilePath;
			std::string animationFilePath;
			bool animIsShared;
			float timeMultiplier;
#ifdef EDITOR_ENABLED
			skeletonFilePath = _config->skeletonFilePath;
			animationFilePath = _config->animationFilePath;
			animIsShared = _config->isShared;
			timeMultiplier = _config->timeMultiplier;
#else
			skeletonFilePath = _skeletonFilePath;
			animationFilePath = _animationFilePath;
			animIsShared = _animIsShared;
			timeMultiplier = _timeMultiplier;
#endif
			ar(skeletonFilePath);
			ar(animationFilePath);
			ar(animIsShared);
			ar(timeMultiplier);
		}

		template <class Archive, cereal::traits::DisableIf<cereal::traits::is_text_archive<Archive>::value> = cereal::traits::sfinae>
		void load(Archive &ar, const std::uint32_t version)
		{
			ar(_skeletonFilePath);
			ar(_animationFilePath);
			ar(_animIsShared);
			ar(_timeMultiplier);
		}

		// !Serialization
		////
		//////


	};
}

CEREAL_CLASS_VERSION(AGE::AnimatedSklComponent, 0)