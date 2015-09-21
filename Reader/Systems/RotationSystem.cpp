#include "RotationSystem.hpp"
#include <ComponentsCore/RotationComponent.hpp>
#include <Entity/Entity.hh>
#include <Entity/EntityData.hh>
#include <Core/Link.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace AGE
{
	RotationSystem::RotationSystem(AScene *scene)
		: System(std::move(scene))
		, _filter(std::move(scene))
	{
		_name = "rotation_system";
	}

	RotationSystem::~RotationSystem()
	{}

	void RotationSystem::updateBegin(float time)
	{}

	void RotationSystem::updateEnd(float time)
	{}

	void RotationSystem::mainUpdate(float time)
	{
		auto &collection = _filter.getCollection();
		for (auto &e : collection)
		{

			glm::vec3 rotation = e->getComponent<RotationComponent>()->_angles;
			glm::quat quaternion = e->getLink().getOrientation();
			auto res = glm::rotate(quaternion, glm::radians(time * e->getComponent<RotationComponent>()->_speed), rotation);
			e->getLink().setOrientation(res);
		}
	}

	bool RotationSystem::initialize()
	{
		_filter.requireComponent<RotationComponent>();
		return true;
	}
}