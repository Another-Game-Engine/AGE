#include "RenderCameraSystem.hpp"
#include <Components/CameraComponent.hpp>
#include <Components/SpotLight.hh>

#include <Core/Engine.hh>
#include <Core/AScene.hh>

#include <BFC/BFCLinkTracker.hpp>
#include <BFC/BFCBlockManagerFactory.hpp>
#include <BFC/BFCCullableObject.hpp>

#include <Graphic/DRBCameraDrawableList.hpp>
#include <Graphic/BFCCullableTypes.hpp>

#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>

namespace AGE
{
	RenderCameraSystem::RenderCameraSystem(AScene *scene) :
		System(std::move(scene)),
		_cameras(std::move(scene)),
		_spotLights(std::move(scene))
	{
		_name = "Camera system";
	}

	bool RenderCameraSystem::initialize()
	{
		_cameras.requireComponent<CameraComponent>();
		_spotLights.requireComponent<SpotLightComponent>();
		return (true);
	}

	void RenderCameraSystem::updateBegin(float time)
	{

	}

	void RenderCameraSystem::mainUpdate(float time)
	{
		_scene->getBfcLinkTracker()->reset();

		std::list<std::shared_ptr<DRBSpotLightDrawableList>> spotLightList;

		for (auto spotEntity : _spotLights.getCollection())
		{
			auto spot = spotEntity->getComponent<SpotLightComponent>();
			auto spotDrawableList = std::make_shared<DRBSpotLightDrawableList>();
			spotDrawableList->spotLight = spot->getCullableHandle().getPtr()->getDatas();
			_scene->getBfcBlockManagerFactory()->cullOnChannel(BFCCullableType::CullableMesh, spotDrawableList->meshs);
			spotLightList.push_back(spotDrawableList);
		}

		for (auto cameraEntity : _cameras.getCollection())
		{
			auto camera = cameraEntity->getComponent<CameraComponent>();

			auto cameraList = std::make_shared<DRBCameraDrawableList>();
			cameraList->cameraInfos.data = camera->getData();
			cameraList->cameraInfos.view = glm::inverse(cameraEntity->getLink().getGlobalTransform());
			_scene->getBfcBlockManagerFactory()->cullOnChannel(BFCCullableType::CullableMesh, cameraList->meshs);
			_scene->getBfcBlockManagerFactory()->cullOnChannel(BFCCullableType::CullablePointLight, cameraList->pointLights);
			cameraList->spotLights = spotLightList;
			AGE::GetRenderThread()->getQueue()->emplaceCommand<AGE::DRBCameraDrawableListCommand>(cameraList);
		}
	}

	void RenderCameraSystem::updateEnd(float time)
	{
	}
}