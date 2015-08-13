#include "RenderCameraSystem.hpp"

#include <Components/CameraComponent.hpp>
#include <Components/SpotLight.hh>
#include <Components/DirectionalLightComponent.hh>
#include <Components/Light.hh>

#include <Core/Engine.hh>
#include <Core/AScene.hh>

#include <glm/gtc/matrix_transform.hpp>

#include <BFC/BFCLinkTracker.hpp>
#include <BFC/BFCBlockManagerFactory.hpp>
#include <BFC/BFCCullableObject.hpp>

#include <Graphic/DRBCameraDrawableList.hpp>
#include <Graphic/BFCCullableTypes.hpp>

#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/Commands/ToRenderCommands.hpp>

#include "Utils/Frustum.hh"

namespace AGE
{
	RenderCameraSystem::RenderCameraSystem(AScene *scene) :
		System(std::move(scene)),
		_cameras(std::move(scene)),
		_spotLights(std::move(scene)),
		_directionnalLights(std::move(scene)),
		_pointLights(std::move(scene))
	{
		_name = "Camera system";
	}

	bool RenderCameraSystem::initialize()
	{
		_cameras.requireComponent<CameraComponent>();
		_spotLights.requireComponent<SpotLightComponent>();
		_directionnalLights.requireComponent<DirectionalLightComponent>();
		_pointLights.requireComponent<PointLightComponent>();
		return (true);
	}

	void RenderCameraSystem::updateBegin(float time)
	{

	}

	void RenderCameraSystem::mainUpdate(float time)
	{
		_scene->getBfcLinkTracker()->reset();

		// check if the render thread does not already have stuff to draw
		if (GetMainThread()->isRenderFrame() == false)
		{
			return;
		}

		std::list<std::shared_ptr<DRBSpotLightDrawableList>> spotLightList;
		std::list<std::shared_ptr<DRBData>> pointLightList;


		for (auto spotEntity : _spotLights.getCollection())
		{
			auto spot = spotEntity->getComponent<SpotLightComponent>();
			auto spotDrawableList = std::make_shared<DRBSpotLightDrawableList>();
			spotDrawableList->spotLight = spot->getCullableHandle().getPtr()->getDatas();

			float spotFov = glm::max(0.001f, (1.0f - spot->getCutOff()) * 180.0f);
			glm::mat4 spotViewProj = glm::perspective(spotFov, 1.0f, 0.1f, 1000.0f) * glm::inverse(spotEntity->getLink().getGlobalTransform());

			Frustum spotlightFrustum;
			spotlightFrustum.setMatrix(spotViewProj);

			// Draw spotlight frustum debug:
			glm::vec4 worldPos = glm::inverse(spotViewProj) * glm::vec4(-1, -1, -1, 1.0f);
			glm::vec3 aNear = glm::vec3(worldPos / worldPos.w);
			worldPos = glm::inverse(spotViewProj) * glm::vec4(-1, 1, -1, 1.0f);
			glm::vec3 bNear = glm::vec3(worldPos / worldPos.w);
			worldPos = glm::inverse(spotViewProj) * glm::vec4(1, 1, -1, 1.0f);
			glm::vec3 cNear = glm::vec3(worldPos / worldPos.w);
			worldPos = glm::inverse(spotViewProj) * glm::vec4(1, -1, -1, 1.0f);
			glm::vec3 dNear = glm::vec3(worldPos / worldPos.w);
			worldPos = glm::inverse(spotViewProj) * glm::vec4(-1, -1, 1, 1.0f);
			glm::vec3 aFar = glm::vec3(worldPos / worldPos.w);
			worldPos = glm::inverse(spotViewProj) * glm::vec4(-1, 1, 1, 1.0f);
			glm::vec3 bFar = glm::vec3(worldPos / worldPos.w);
			worldPos = glm::inverse(spotViewProj) * glm::vec4(1, 1, 1, 1.0f);
			glm::vec3 cFar = glm::vec3(worldPos / worldPos.w);
			worldPos = glm::inverse(spotViewProj) * glm::vec4(1, -1, 1, 1.0f);
			glm::vec3 dFar = glm::vec3(worldPos / worldPos.w);

			AGE::GetRenderThread()->getQueue()->emplaceCommand<Commands::ToRender::Draw3DQuad>(aNear, bNear, cNear, dNear);
			AGE::GetRenderThread()->getQueue()->emplaceCommand<Commands::ToRender::Draw3DQuad>(aFar, bFar, cFar, dFar);
			AGE::GetRenderThread()->getQueue()->emplaceCommand<Commands::ToRender::Draw3DLine>(aNear, aFar);
			AGE::GetRenderThread()->getQueue()->emplaceCommand<Commands::ToRender::Draw3DLine>(bNear, bFar);
			AGE::GetRenderThread()->getQueue()->emplaceCommand<Commands::ToRender::Draw3DLine>(cNear, cFar);
			AGE::GetRenderThread()->getQueue()->emplaceCommand<Commands::ToRender::Draw3DLine>(dNear, dFar);

			_scene->getBfcBlockManagerFactory()->cullOnChannel(BFCCullableType::CullableMesh, spotDrawableList->meshs, spotlightFrustum);
			spotLightList.push_back(spotDrawableList);
		}
		for (auto pointLightEntity : _pointLights.getCollection())
		{
			auto point = pointLightEntity->getComponent<PointLightComponent>();
			
			pointLightList.push_back(point->getCullableHandle().getPtr()->getDatas());
		}

		for (auto cameraEntity : _cameras.getCollection())
		{
			Frustum cameraFrustum;
			auto camera = cameraEntity->getComponent<CameraComponent>();

			auto cameraList = std::make_shared<DRBCameraDrawableList>();
			cameraList->cameraInfos.data = camera->getData();
			cameraList->cameraInfos.view = glm::inverse(cameraEntity->getLink().getGlobalTransform());

			cameraFrustum.setMatrix(camera->getProjection() * cameraList->cameraInfos.view);

			_scene->getBfcBlockManagerFactory()->cullOnChannel(BFCCullableType::CullableMesh, cameraList->meshs, cameraFrustum);
			_scene->getBfcBlockManagerFactory()->cullOnChannel(BFCCullableType::CullablePointLight, cameraList->pointLights, cameraFrustum);
			cameraList->spotLights = spotLightList;
			cameraList->pointLights = pointLightList;
			AGE::GetRenderThread()->getQueue()->emplaceCommand<AGE::DRBCameraDrawableListCommand>(cameraList);
		}
	}

	void RenderCameraSystem::updateEnd(float time)
	{
	}
}