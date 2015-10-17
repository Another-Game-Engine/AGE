#include "AnimatedSklSystem.hpp"

#include <Entity/Entity.hh>
#include <Entity/EntityData.hh>

#include "Skinning/AnimatedSklComponent.hpp"
#include "Skinning/Skeleton.hpp"
#include "Skinning/AnimationManager.hpp"

#include "AssetManagement/Instance/AnimationInstance.hh"

#include "Core/AScene.hh"

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
		SCOPE_profile_cpu_function("Animations");

		static float hackTime = 0;
		hackTime += time;

		auto animationManager = _scene->getInstance<AnimationManager>();
		AGE_ASSERT(animationManager != nullptr);
		animationManager->update(hackTime);

		auto &collection = _filter.getCollection();
		for (auto &e : collection)
		{
			auto skelCpt = e->getComponent<AnimatedSklComponent>();
			auto meshCpt = e->getComponent<MeshRenderer>();

			auto animation = skelCpt->getAnimation();
			if (animation)
			{
				// TODO
				meshCpt->setSkinningMatrix(animation->getTransformations(), animation->getTransformationBufferSize());
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