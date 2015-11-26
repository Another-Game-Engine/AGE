#include "BasicDemoCameraSystem.hh"

#include <ComponentsCore/RigidBody.hpp>
#include <ComponentsCore/Collider.hpp>
#include <ComponentsCore/CameraComponent.hpp>
#include <ComponentsCore/Light.hh>
#include <ComponentsCore/MeshRenderer.hh>
#include <Components/Lifetime.hpp>

#include <Entity/Entity.hh>
#include <Entity/EntityData.hh>

#include <Core/AScene.hh>
#include <SystemsCore/PhysicsSystem.hpp>

#include <Physics/RaycasterInterface.hpp>
#include <Physics/RaycastHit.hpp>

#include <Physics/CharacterControllerInterface.hh>
#include <Core/Inputs/Input.hh>

#include <SystemsCore/RenderCameraSystem.hpp>
#include <imgui/imgui.h>

#include <AssetManagement/AssetManager.hh>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace AGE
{
	BasicDemoCameraSystem::BasicDemoCameraSystem(AScene *scene) :
		System(scene),
		_filter(scene)
	{
	}

	bool BasicDemoCameraSystem::initialize()
	{
		_filter.requireComponent<CameraComponent>();
		return (true);
	}

	void BasicDemoCameraSystem::updateBegin(float time)
	{
	}

	void BasicDemoCameraSystem::mainUpdate(float time)
	{
		for (Entity const &entity : _filter.getCollection())
		{
			static float trigger = 1.0f;
			Joystick controller;
			auto inputs = _scene->getInstance<Input>();
			bool controllerEnabled = inputs->getJoystick(0, controller);
			if ((inputs->getPhysicalKeyPressed(AgeKeys::AGE_SPACE) || (controllerEnabled && controller.getButtonPressed(AgeJoystickButtons::AGE_JOYSTICK_BUTTON_RIGHTSHOULDER))) && trigger >= 0.15f)
			{
				trigger = 0.0f;
				auto e = _scene->createEntity();
				e->addComponent<Lifetime>(15.0f);
				auto &link = e->getLink();
				auto &cameraLink = entity->getLink();
				const glm::quat &cameraOrientation = cameraLink.getOrientation();
				const glm::vec3 cameraForward = glm::vec3(glm::mat4(glm::toMat4(cameraOrientation) * glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -1.0f)))[3]);
				link.setPosition(cameraLink.getPosition() + cameraOrientation * glm::vec3(0, 0, -2));
				link.setOrientation(cameraOrientation);
				link.setScale(glm::vec3(1.0f));
				e->addComponent<MeshRenderer>(_scene->getInstance<AGE::AssetsManager>()->getMesh("cube/cube.sage"),
											 _scene->getInstance<AGE::AssetsManager>()->getMaterial(OldFile("cube/cube.mage")))->enableRenderMode(RenderModes::AGE_OPAQUE);
				e->addComponent<RigidBody>()->addForce(10.0f * cameraForward, Physics::ForceMode::Impulse);
				e->addComponent<Collider>(Physics::ColliderType::Box);
				auto pl = e->addComponent<PointLightComponent>();
				pl->setColor(glm::vec3(float(rand() % 100) / 100.0f, float(rand() % 100) / 100.0f, float(rand() % 100) / 100.0f));
			}

			trigger += time;

#if defined(EDITOR_ENABLED)
			entity->getComponent<CameraComponent>()->editorUpdate();

			auto camComponent = entity->getComponent<CameraComponent>();
			static char const *pipelineNames[RenderType::TOTAL] = { "Debug deferred rendering", "Deferred rendering" };
			ImGui::ListBox("Pipelines", &_pipelineIndex, pipelineNames, int(RenderType::TOTAL));
			if (camComponent->getPipeline() != (RenderType)_pipelineIndex)
			{
				camComponent->setPipeline((RenderType)_pipelineIndex);
				_scene->getSystem<RenderCameraSystem>()->drawDebugLines(_pipelineIndex == (int)RenderType::DEBUG_DEFERRED ? true : false);
			}
#endif
		}
	}

	void BasicDemoCameraSystem::updateEnd(float time)
	{
	}
}
