#include "RenderCameraSystem.hpp"

#include <ComponentsCore/CameraComponent.hpp>
#include <ComponentsCore/SpotLight.hh>
#include <ComponentsCore/DirectionalLightComponent.hh>
#include <ComponentsCore/Light.hh>

#include <Core/Engine.hh>
#include <Core/AScene.hh>

#include <glm/gtc/matrix_transform.hpp>

#include <BFC/BFCLinkTracker.hpp>
#include <BFC/BFCBlockManagerFactory.hpp>
#include <BFC/BFCCullableObject.hpp>

#include <BFC/BFCCuller.hpp>
#include <BFC/BFCFrustumCuller.hpp>

#include <Graphic/DRBCameraDrawableList.hpp>
#include <Graphic/DRBSpotLightData.hpp>
#include <Graphic/BFCCullableTypes.hpp>
#include <Graphic/DRBMeshData.hpp>
#include <Graphic/DRBMesh.hpp>
#include <Graphic/DRBSkinnedMesh.hpp>
#include <Graphic/DRBPointLight.hpp>
#include <Graphic/DRBSpotLight.hpp>

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

#include <algorithm>

#include "Render/Pipelining/Pipelines/IrenderingPipeline.hh"
#include "Render/Pipelining/RenderInfos/SpotlightRenderInfos.hpp"

//hack
#include "Render\Pipelining\Pipelines\CustomRenderPass\DeferredShadowBuffering.hh"
#include "Render\Pipelining\Pipelines\CustomRenderPass\DeferredBasicBuffering.hh"

namespace AGE
{
	RenderCameraSystem::RenderCameraSystem(AScene *scene) :
		System(std::move(scene)),
		_cameras(std::move(scene)),
		_spotLights(std::move(scene)),
		_directionnalLights(std::move(scene)),
		_pointLights(std::move(scene)),
		_drawDebugLines(false),
		_cullingEnabled(true)
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
			if (mesh->hadRenderMode(AGE_OCCLUDER) || mesh->hadRenderMode(AGE_SKINNED))
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
				TMQ::TaskManager::emplaceRenderTask<AGE::Commands::ToRender::Draw2DQuad>(glm::vec2(minPoint.x, minPoint.y), glm::vec2(minPoint.x, maxPoint.y), glm::vec2(maxPoint.x, maxPoint.y), glm::vec2(maxPoint.x, minPoint.y), glm::vec3(0, 1, 0));
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

		// if no camera
		if (_cameras.getCollection().empty())
		{
			return;
		}

		// Get the first camera (only one camera is supported for the moment
		auto firstCameraEntity = *(_cameras.getCollection().begin());
		auto firstCameraView = glm::inverse(firstCameraEntity->getLink().getGlobalTransform());
		auto firstCameraComponent = firstCameraEntity->getComponent<CameraComponent>();
		auto &firstCameraData = firstCameraComponent->getData();
		auto &cameraPipeline = GetRenderThread()->pipelines[firstCameraData.pipeline];
		auto spotlightInfos = cameraPipeline->getSpotlightRenderInfos();

		std::list<std::shared_ptr<DRBPointLightData>> pointLightList;

		std::list<std::atomic_uint64_t*> spotCounters;
		std::list<BFCCuller<BFCFrustumCuller>> spotCullers;

		if (spotlightInfos != nullptr)
		{
			// we set camera infos
			spotlightInfos->setCameraInfos(firstCameraComponent->getProjection(), firstCameraView);

			SCOPE_profile_cpu_i("Camera system", "Cull for spots");
			for (auto &spotEntity : _spotLights.getCollection())
			{
				SCOPE_profile_cpu_i("Camera system", "Spot");

				auto spot = spotEntity->getComponent<SpotLightComponent>();

				glm::mat4 spotViewProj = spot->updateShadowMatrix();
				Frustum spotlightFrustum;
				spotlightFrustum.setMatrix(spotViewProj);
				
				// we set spot infos
				spotlightInfos->setSpotlightInfos(
					spot->getPosition(),
					spot->getAttenuation(),
					spot->getDirection(),
					spot->getColor(),
					spotViewProj,
					spot->getCutOff(),
					spot->getExponent());

				BFCBlockManagerFactory *bf = _scene->getBfcBlockManagerFactory();

				//We create a culler, with the culling rule "Frustum"
				spotCullers.emplace_back();
				auto &spotCuller = spotCullers.back();
				//We pass infos for frustum culling
				spotCuller.prepareForCulling(spotlightFrustum);
				//We get an output of a specific type
				//here it's for mesh for basic buffering pass
				auto meshOutput = SpotlightRenderInfos::MeshOutput::GetNewOutput();
				auto skinnedOutput = SpotlightRenderInfos::SkinnedOutput::GetNewOutput();
				//We get the ptr of the queue where the output should be push at the end of the
				//culling and preparation process
				auto meshResultQueue = spotlightInfos->getMeshResultQueue();
				auto skinnedResultQueue = spotlightInfos->getSkinnedResultQueue();
				meshOutput->setResultQueue(meshResultQueue);
				skinnedOutput->setResultQueue(skinnedResultQueue);
				meshOutput->getCommandOutput()._spotLightMatrix = spotViewProj;
				skinnedOutput->getCommandOutput()._spotLightMatrix = spotViewProj;

				spotCuller.addOutput(BFCCullableType::CullableMesh, meshOutput);
				spotCuller.addOutput(BFCCullableType::CullableSkinnedMesh, skinnedOutput);
				auto counter = spotCuller.cull(bf);
				spotCounters.push_back(counter);
			}
		}

		for (auto &pointLightEntity : _pointLights.getCollection())
		{
			auto point = pointLightEntity->getComponent<PointLightComponent>();
			
			pointLightList.push_back(point->getCullableHandle().getPtr<DRBPointLight>()->getDatas());
		}

		///////////////////////////////
		/// BLOCKING WAIT FOR SPOTS
		{
			SCOPE_profile_cpu_i("Camera system", "Cull for spots wait");
			std::size_t tot = 1;
			while (tot > 0)
			{
				tot = 0;
				for (auto &e : spotCounters)
					tot += e->load();
			}
		}

		for (auto &cameraEntity : _cameras.getCollection())
		{
			SCOPE_profile_cpu_i("Camera system", "Cull for cam");

			Frustum cameraFrustum;
			auto camera = cameraEntity->getComponent<CameraComponent>();

			std::atomic_size_t counter = 0;
			std::atomic_size_t MESH_COUNTER = 0;

			auto cameraList = std::make_shared<DRBCameraDrawableList>();
			cameraList->cameraInfos.data = camera->getData();
			cameraList->cameraInfos.view = glm::inverse(cameraEntity->getLink().getGlobalTransform());

			cameraFrustum.setMatrix(camera->getProjection() * cameraList->cameraInfos.view);

			LFList<BFCItem> pointLightListToCull;
			std::size_t pointLightBlocksToCullNumber = _scene->getBfcBlockManagerFactory()->getBlockNumberToCull(BFCCullableType::CullablePointLight);
			counter = /*meshBlocksToCullNumber +*/ pointLightBlocksToCullNumber /*+ skinnedMeshBlocksToCullNumber*/;

			BFCBlockManagerFactory *bf = _scene->getBfcBlockManagerFactory();
			if (DeferredBasicBuffering::instance)
			{
				//We create a culler, with the culling rule "Frustum"
				BFCCuller<BFCFrustumCuller> cameraCuller;
				//We pass infos for frustum culling
				cameraCuller.prepareForCulling(cameraFrustum);
				//We get an output of a specific type
				//here it's for mesh for basic buffering pass
				auto meshOutput = DeferredBasicBuffering::MeshOutput::GetNewOutput();
				auto skinnedMeshOutput = DeferredBasicBuffering::SkinnedMeshOutput::GetNewOutput();
				cameraList->cameraMeshs = meshOutput;
				cameraList->cameraSkinnedMeshs = skinnedMeshOutput;
				//We get the ptr of the queue where the output should be push at the end of the
				//culling and preparation process
				auto meshResultQueue = DeferredBasicBuffering::instance->getMeshResultQueue();
				auto skinnedMeshResultQueue = DeferredBasicBuffering::instance->getSkinnedMeshResultQueue();
				meshOutput->setResultQueue(meshResultQueue);
				skinnedMeshOutput->setResultQueue(skinnedMeshResultQueue);
				cameraCuller.addOutput(BFCCullableType::CullableMesh, meshOutput);
				cameraCuller.addOutput(BFCCullableType::CullableSkinnedMesh, skinnedMeshOutput);
				auto counter = cameraCuller.cull(bf);
				while (counter->load() > 0)
				{ }
				//cameraList->cameraMeshs = DeferredBasicBuffering::instance->prepareRender(camera->getProjection(), bf, cameraFrustum, &MESH_COUNTER);
			}

			{
				SCOPE_profile_cpu_i("Camera system", "Cull for pointlights");

				for (std::size_t i = 0; i < pointLightBlocksToCullNumber; ++i)
				{
					BFCBlockManagerFactory *bf = _scene->getBfcBlockManagerFactory();
					TMQ::TaskManager::emplaceSharedTask<Tasks::Basic::VoidFunction>([bf, i, &cameraFrustum, &counter, &pointLightListToCull](){
						counter.fetch_sub(bf->cullOnBlock(BFCCullableType::CullablePointLight, pointLightListToCull, cameraFrustum, i, 1));
					});
				}
			}

			{
				SCOPE_profile_cpu_i("Camera system", "Cull for cam wait");
				while (counter.load() > 0 || MESH_COUNTER.load() > 0)
				{ }
				//GetMainThread()->computeTasksWhile(std::function<bool()>([&counter, totalToCullNumber]() {
				//	return counter >= totalToCullNumber;
				//}));
			}

			{
				SCOPE_profile_cpu_i("Camera system", "Copy LFList to std");

				while (pointLightListToCull.getSize() > 0)
				{
					cameraList->pointLights.push_back(((DRBPointLight*)(pointLightListToCull.pop()->getDrawable()))->getDatas());
				}
			}
			if (OcclusionConfig::g_Occlusion_is_enabled)
			{
				occlusionCulling(cameraList->meshs, _drawDebugLines);
			}

			cameraList->pointLights = pointLightList;
			TMQ::TaskManager::emplaceRenderTask<AGE::DRBCameraDrawableListCommand>(cameraList);
		}

	}

	void RenderCameraSystem::updateEnd(float time)
	{
	}
}