#include "AnimatedSklSystem.hpp"

#include <Entity/Entity.hh>
#include <Entity/EntityData.hh>

#include "AnimatedSklComponent.hpp"
#include "Skeleton.hpp"

#include "AssetManagement/Instance/AnimationInstance.hh"

#include <ComponentsCore/MeshRenderer.hh>

namespace AGE
{
	AnimatedSklSystem::AnimatedSklSystem(AScene *scene)
		: System(std::move(scene))
		, _filter(std::move(scene))
	{
		_name = "animated_skel_system";
	}

	AnimatedSklSystem::~AnimatedSklSystem()
	{}

	void AnimatedSklSystem::updateBegin(float time)
	{}

	void AnimatedSklSystem::updateEnd(float time)
	{}

	void AnimatedSklSystem::mainUpdate(float time)
	{
		static float hackTime = 0;
		hackTime += time;

		auto &collection = _filter.getCollection();
		for (auto &e : collection)
		{
			auto skelCpt = e->getComponent<AnimatedSklComponent>();
			auto meshCpt = e->getComponent<MeshRenderer>();

			auto animation = skelCpt->getAnimation();
			if (animation)
			{
				animation->update(hackTime);
				skelCpt->getSkeleton()->updateSkinning();
				meshCpt->setSkinningMatrix(animation->getBones());
			}
		}
	}

	bool AnimatedSklSystem::initialize()
	{
		_filter.requireComponent<AnimatedSklComponent>();
		_filter.requireComponent<MeshRenderer>();
		return true;
	}
}