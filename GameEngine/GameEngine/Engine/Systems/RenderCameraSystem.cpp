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
#include <Graphic/DRBMeshData.hpp>

#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/MainThread.hpp>

#include "Utils/Frustum.hh"
#include "Utils/AABoundingBox.hh"

#include <Render/OcclusionTools/DepthMap.hpp>
#include <Render/OcclusionTools/DepthMapManager.hpp>
#include <Render/OcclusionTools/DepthMapHandle.hpp>

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

	void occlusionCulling(std::list<std::shared_ptr<DRBData>> &list)
	{
		SCOPE_profile_cpu_function("Camera system");

		auto depthMap = GetRenderThread()->getDepthMapManager().getReadableMap();
		//for (auto &e : cameraList->meshs)
		//{
		//	readableDepthMap->testBox()
		//}

		if (depthMap.isValid() == false)
		{
			return;
		}

		auto j = list.begin();
		while (j != std::end(list))
		{
			auto &d = *j;
			auto BB = std::static_pointer_cast<DRBMeshData>(d)->getAABB();

			glm::vec2 minPoint = glm::vec2(1);
			glm::vec2 maxPoint = glm::vec2(-1);

			float minZ = std::numeric_limits<float>::max();

			for (std::size_t i = 0; i < 8; ++i)
			{
				auto point = depthMap->getMV() * d->getTransformation() * glm::vec4(BB.getCornerPoint(i), 1.0f);
				point /= point.w;

				if (point.x < -1)
				{
					point.x = -1;
				}
				if (point.y < -1)
				{
					point.y = -1;
				}
				if (point.x > 1)
				{
					point.x = 1;
				}
				if (point.y > 1)
				{
					point.y = 1;
				}

				minPoint.x = std::min(minPoint.x, point.x);
				minPoint.y = std::min(minPoint.y, point.y);
				maxPoint.x = std::max(maxPoint.x, point.x);
				maxPoint.y = std::max(maxPoint.y, point.y);

				point.z = (point.z + 1.0f) * 0.5f;
				minZ = std::min(minZ, point.z);
			}

			glm::uvec2 screenMin(((minPoint + glm::vec2(1)) / glm::vec2(2)) * glm::vec2(depthMap->getMipmapWidth(), depthMap->getMipmapHeight()));
			glm::uvec2 screenMax(((maxPoint + glm::vec2(1)) / glm::vec2(2)) * glm::vec2(depthMap->getMipmapWidth(), depthMap->getMipmapHeight()));

			if (minZ < 0)
			{
				minZ = 0;
			}

			if (depthMap->testBox((uint32_t)(minZ * (1 << 24)), screenMin, screenMax) == false)
			{
				list.erase(j++);
			}
			else
			{
				++j;
			}
		}
	}

	void RenderCameraSystem::mainUpdate(float time)
	{
		SCOPE_profile_cpu_function("Camera system");
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

			// WARNINNNNNNNNNNNG
			// Le calcul n'est pas bon !!!!
			// Paul, fais le STP, merci
			Frustum spotlightFrustum;
			spotlightFrustum.setMatrix(glm::perspective(spot->getCutOff() / 2.0f, spot->getExponent(), 0.1f, 1000.0f)* glm::inverse(spotEntity->getLink().getGlobalTransform()));

			LFList<BFCItem> list;
			_scene->getBfcBlockManagerFactory()->cullOnChannel(BFCCullableType::CullableMesh, list/*spotDrawableList->meshs*/, spotlightFrustum);
			while (list.getSize() > 0)
			{
				spotDrawableList->meshs.push_back(list.pop()->getDrawable()->getDatas());
			}
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

			LFList<BFCItem> list;
			_scene->getBfcBlockManagerFactory()->cullOnChannel(BFCCullableType::CullableMesh, list/*cameraList->meshs*/, cameraFrustum);
			while (list.getSize() > 0)
			{
				cameraList->meshs.push_back(list.pop()->getDrawable()->getDatas());
			}
			_scene->getBfcBlockManagerFactory()->cullOnChannel(BFCCullableType::CullablePointLight, list/*cameraList->pointLights*/, cameraFrustum);
			while (list.getSize() > 0)
			{
				cameraList->pointLights.push_back(list.pop()->getDrawable()->getDatas());
			}
			occlusionCulling(cameraList->meshs);

			cameraList->spotLights = spotLightList;
			cameraList->pointLights = pointLightList;
			AGE::GetRenderThread()->getQueue()->emplaceCommand<AGE::DRBCameraDrawableListCommand>(cameraList);
		}
	}

	void RenderCameraSystem::updateEnd(float time)
	{
	}
}