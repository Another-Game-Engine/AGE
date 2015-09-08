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
#include <Graphic/DRBSpotLightData.hpp>
#include <Graphic/BFCCullableTypes.hpp>
#include <Graphic/DRBMeshData.hpp>

#include <Threads/ThreadManager.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/MainThread.hpp>
#include <Threads/Commands/ToRenderCommands.hpp>

#include "Utils/Frustum.hh"
#include "Utils/AABoundingBox.hh"

#include <Render/OcclusionTools/DepthMap.hpp>
#include <Render/OcclusionTools/DepthMapManager.hpp>
#include <Render/OcclusionTools/DepthMapHandle.hpp>

#include <Threads/TaskScheduler.hpp>
#include <Threads/Tasks/BasicTasks.hpp>

#include <render/OcclusionTools/OcclusionOptions.hpp>

//temp
#include <chrono>

namespace AGE
{
	RenderCameraSystem::RenderCameraSystem(AScene *scene) :
		System(std::move(scene)),
		_cameras(std::move(scene)),
		_spotLights(std::move(scene)),
		_directionnalLights(std::move(scene)),
		_pointLights(std::move(scene)),
		_drawDebugLines(false)
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

	void RenderCameraSystem::drawDebugLines(bool activated)
	{
		_drawDebugLines = activated;
	}

	void occlusionCulling(std::list<std::shared_ptr<DRBData>> &list, bool drawDebugLines)
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
			auto mesh = std::static_pointer_cast<DRBMeshData>(d);
			if (mesh->hadRenderMode(AGE_OCCLUDER))
			{
				++j;
				continue;
			}
			auto BB = mesh->getAABB();


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
				continue;
			}
			if (drawDebugLines)
			{
				GetRenderThread()->getQueue()->emplaceTask<AGE::Commands::ToRender::Draw2DQuad>(glm::vec2(minPoint.x, minPoint.y), glm::vec2(minPoint.x, maxPoint.y), glm::vec2(maxPoint.x, maxPoint.y), glm::vec2(maxPoint.x, minPoint.y), glm::vec3(0, 1, 0));
			}
			++j;
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


		for (auto &spotEntity : _spotLights.getCollection())
		{
			auto spot = spotEntity->getComponent<SpotLightComponent>();
			auto spotDrawableList = std::make_shared<DRBSpotLightDrawableList>();
			spotDrawableList->spotLight = spot->getCullableHandle().getPtr()->getDatas();

			auto spotData = std::static_pointer_cast<DRBSpotLightData>(spotDrawableList->spotLight);

			glm::mat4 spotViewProj = spot->updateShadowMatrix();

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

			glm::vec3 color = glm::vec3(1, 0, 0);
			bool activateDepth = true;

			if (_drawDebugLines)
			{
				AGE::GetRenderThread()->getQueue()->emplaceTask<Commands::ToRender::Draw3DQuad>(aNear, bNear, cNear, dNear, color, activateDepth);
				AGE::GetRenderThread()->getQueue()->emplaceTask<Commands::ToRender::Draw3DQuad>(aFar, bFar, cFar, dFar, color, activateDepth);
				AGE::GetRenderThread()->getQueue()->emplaceTask<Commands::ToRender::Draw3DLine>(aNear, aFar, color, activateDepth);
				AGE::GetRenderThread()->getQueue()->emplaceTask<Commands::ToRender::Draw3DLine>(bNear, bFar, color, activateDepth);
				AGE::GetRenderThread()->getQueue()->emplaceTask<Commands::ToRender::Draw3DLine>(cNear, cFar, color, activateDepth);
				AGE::GetRenderThread()->getQueue()->emplaceTask<Commands::ToRender::Draw3DLine>(dNear, dFar, color, activateDepth);
			}

			std::atomic_size_t counter = 0;

			LFList<BFCItem> meshInLightList;
			std::size_t meshBlocksToCullNumber = _scene->getBfcBlockManagerFactory()->getBlockNumberToCull(BFCCullableType::CullableMesh);

			for (std::size_t i = 0; i < meshBlocksToCullNumber; ++i)
			{
				BFCBlockManagerFactory *bf = _scene->getBfcBlockManagerFactory();
				EmplaceTask<Tasks::Basic::VoidFunction>([bf, i, &spotlightFrustum, &counter, &meshInLightList](){
					bf->cullOnBlock(BFCCullableType::CullableMesh, meshInLightList, spotlightFrustum, i);
					counter.fetch_add(1);
				});
			}

			{
				SCOPE_profile_cpu_i("Camera system", "Cull for spots");
				GetMainThread()->computeTasksWhile(std::function<bool()>([&counter, meshBlocksToCullNumber]() {
					return counter >= meshBlocksToCullNumber;
				}));
			}

			while (meshInLightList.getSize() > 0)
			{
				spotDrawableList->meshs.push_back(meshInLightList.pop()->getDrawable()->getDatas());
			}

			spotLightList.push_back(spotDrawableList);
		}
		for (auto &pointLightEntity : _pointLights.getCollection())
		{
			auto point = pointLightEntity->getComponent<PointLightComponent>();
			
			pointLightList.push_back(point->getCullableHandle().getPtr()->getDatas());
		}

		for (auto &cameraEntity : _cameras.getCollection())
		{
			Frustum cameraFrustum;
			auto camera = cameraEntity->getComponent<CameraComponent>();

			std::atomic_size_t counter = 0;

			auto cameraList = std::make_shared<DRBCameraDrawableList>();
			cameraList->cameraInfos.data = camera->getData();
			cameraList->cameraInfos.view = glm::inverse(cameraEntity->getLink().getGlobalTransform());

			cameraFrustum.setMatrix(camera->getProjection() * cameraList->cameraInfos.view);

			LFList<BFCItem> meshList;
			LFList<BFCItem> pointLightListToCull;
			std::size_t meshBlocksToCullNumber = _scene->getBfcBlockManagerFactory()->getBlockNumberToCull(BFCCullableType::CullableMesh);
			std::size_t pointLightBlocksToCullNumber = _scene->getBfcBlockManagerFactory()->getBlockNumberToCull(BFCCullableType::CullablePointLight);
			std::size_t totalToCullNumber = meshBlocksToCullNumber + pointLightBlocksToCullNumber;
			
			for (std::size_t i = 0; i < meshBlocksToCullNumber; ++i)
			{
				BFCBlockManagerFactory *bf = _scene->getBfcBlockManagerFactory();
				EmplaceTask<Tasks::Basic::VoidFunction>([bf, i, &cameraFrustum, &counter, &meshList](){
					bf->cullOnBlock(BFCCullableType::CullableMesh, meshList, cameraFrustum, i);
					counter.fetch_add(1);
				});
			}

			for (std::size_t i = 0; i < pointLightBlocksToCullNumber; ++i)
			{
				BFCBlockManagerFactory *bf = _scene->getBfcBlockManagerFactory();
				EmplaceTask<Tasks::Basic::VoidFunction>([bf, i, &cameraFrustum, &counter, &pointLightListToCull](){
					bf->cullOnBlock(BFCCullableType::CullablePointLight, pointLightListToCull, cameraFrustum, i);
					counter.fetch_add(1);
				});
			}

			{
				SCOPE_profile_cpu_i("Camera system", "Cull for cam");
				GetMainThread()->computeTasksWhile(std::function<bool()>([&counter, totalToCullNumber]() {
					return counter >= totalToCullNumber;
				}));
			}

			{
				SCOPE_profile_cpu_i("Camera system", "Copy LFList to std");
				while (meshList.getSize() > 0)
				{
					cameraList->meshs.push_back(meshList.pop()->getDrawable()->getDatas());
				}

				while (pointLightListToCull.getSize() > 0)
				{
					cameraList->pointLights.push_back(pointLightListToCull.pop()->getDrawable()->getDatas());
				}
			}
			if (OcclusionConfig::g_Occlusion_is_enabled)
			{
				occlusionCulling(cameraList->meshs, _drawDebugLines);
			}

			cameraList->spotLights = spotLightList;
			cameraList->pointLights = pointLightList;
			AGE::GetRenderThread()->getQueue()->emplaceTask<AGE::DRBCameraDrawableListCommand>(cameraList);
		}
	}

	void RenderCameraSystem::updateEnd(float time)
	{
	}
}