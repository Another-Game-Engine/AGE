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

#include <algorithm>

namespace AGE
{
	class ShadowCasterBFCCallback : public IBFCCullCallback
	{
	public:
		struct MatrixHandler
		{
			ConcatenatedKey key;
			glm::mat4 matrix;
			MatrixHandler() : key(-1), matrix(glm::mat4(1)) {}
			MatrixHandler(MatrixHandler &&o) : key(std::move(o.key)), matrix(std::move(o.matrix)) {}
		};
		ShadowCasterBFCCallback(std::size_t _blockNumber)
			: blockNumber(_blockNumber)
		{
			SCOPE_profile_cpu_function("Camera system");

			AGE_ASSERT(blockNumber > 0);
			matrixKeyArray = new MatrixHandler[blockNumber * MaxItemID]();
			list           = new LFList<BFCItem>[blockNumber];
			skinnedList    = new LFList<BFCItem>[blockNumber];
			totalNotSkinned = 0;
		}

		~ShadowCasterBFCCallback()
		{
			SCOPE_profile_cpu_function("Camera system");

			if (matrixKeyArray)
				delete[] matrixKeyArray;
			delete[] list;
			delete[] skinnedList;
		}

		MatrixHandler   *matrixKeyArray = nullptr;
		LFList<BFCItem> *list = nullptr;
		LFList<BFCItem> *skinnedList = nullptr;
		std::atomic_size_t totalNotSkinned;
		const std::size_t blockNumber;

		virtual void operator()(LFList<BFCItem> &, std::size_t blockId);
		void sortAll();
		void fill(std::vector<DRBSpotLightOccluder> &res, std::list<std::shared_ptr<DRBData>> &skinned);
	};

	inline bool compare(const ShadowCasterBFCCallback::MatrixHandler &a, const ShadowCasterBFCCallback::MatrixHandler &b)
	{
		return (a.key < b.key);
	}

	void ShadowCasterBFCCallback::operator()(LFList<BFCItem> &, std::size_t blockId)
	{
		SCOPE_profile_cpu_function("Camera system");

		auto &listBlock = list[blockId];
		std::size_t index = blockId * MaxItemID;
		while (listBlock.getSize() > 0)
		{
			auto *item = listBlock.pop();
			DRBMeshData * mesh = (DRBMeshData*)(item->getDrawable()->getDatas().get());
			if (mesh->hadRenderMode(AGE_SKINNED))
			{
				skinnedList[blockId].push(item);
			}
			else
			{
				matrixKeyArray[index].key = ConcatenateKey(mesh->getPainterKey(), mesh->getVerticesKey());
				matrixKeyArray[index].matrix = mesh->getTransformation();
				++index;
			}
		}
		totalNotSkinned.fetch_add(index - blockId * MaxItemID);
		std::sort((matrixKeyArray + blockId * MaxItemID), (matrixKeyArray + (blockId + 1) * MaxItemID), compare);
	}

	void ShadowCasterBFCCallback::sortAll()
	{
		SCOPE_profile_cpu_function("Camera system");

		std::sort(matrixKeyArray, (matrixKeyArray + blockNumber * MaxItemID), compare);
	}

	void ShadowCasterBFCCallback::fill(std::vector<DRBSpotLightOccluder> &res, std::list<std::shared_ptr<DRBData>> &skinned)
	{
		SCOPE_profile_cpu_function("Camera system");

		for (auto i = 0; i < blockNumber; ++i)
		{
			while (skinnedList[i].getSize() > 0)
			{
				auto *item = skinnedList[i].pop();
				skinned.push_back(item->getDrawable()->getDatas());
			}
		}

		if (totalNotSkinned == 0)
		{
			return;
		}

		std::size_t max = totalNotSkinned;
		res.resize(max * 2);
		std::size_t i = 0;
		DRBSpotLightOccluder *key = nullptr;
		ConcatenatedKey lastKey = -1;
		std::size_t keyCounter = 0;
		std::size_t j = 0;
		while (i < max)
		{
			auto &c = matrixKeyArray[i];
			if (c.key != lastKey)
			{
				if (key)
				{
					key->keyHolder.size = keyCounter;
				}
				keyCounter = 0;
				lastKey = c.key;
				res[j] = DRBSpotLightOccluder(std::move(c.key));
				key = &(res[j++]);
			}
			res[j++] = DRBSpotLightOccluder(std::move(matrixKeyArray[i].matrix));
			++keyCounter;
			++i;
		}
		if (key)
			key->keyHolder.size = keyCounter;
		res.resize(j);
	}


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

			std::size_t meshBlocksToCullNumber = _scene->getBfcBlockManagerFactory()->getBlockNumberToCull(BFCCullableType::CullableMesh);

			if (meshBlocksToCullNumber > 0)
			{
				ShadowCasterBFCCallback shadowCaster(meshBlocksToCullNumber);

				for (std::size_t i = 0; i < meshBlocksToCullNumber; ++i)
				{
					BFCBlockManagerFactory *bf = _scene->getBfcBlockManagerFactory();
					EmplaceTask<Tasks::Basic::VoidFunction>([bf, i, &spotlightFrustum, &counter, &shadowCaster]()
					{
						auto &list = shadowCaster.list[i];
						bf->cullOnBlock(BFCCullableType::CullableMesh, list, spotlightFrustum, i, &shadowCaster);
						counter.fetch_add(1);
					});
				}

				{
					SCOPE_profile_cpu_i("Camera system", "Cull for spots");
					while (counter < meshBlocksToCullNumber)
					{
					}
				}

				shadowCaster.sortAll();
				shadowCaster.fill(spotDrawableList->occluders, spotDrawableList->skinnedMesh);
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

				if (_cullingEnabled)
				{
					EmplaceTask<Tasks::Basic::VoidFunction>([bf, i, &cameraFrustum, &counter, &meshList](){
						bf->cullOnBlock(BFCCullableType::CullableMesh, meshList, cameraFrustum, i);
						counter.fetch_add(1);
					});
				}
				else
				{
					EmplaceTask<Tasks::Basic::VoidFunction>([bf, i, &counter, &meshList](){
						bf->fillOnBlock(BFCCullableType::CullableMesh, meshList, i);
						counter.fetch_add(1);
					});
				}
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
				while (counter < totalToCullNumber)
				{ }
				//GetMainThread()->computeTasksWhile(std::function<bool()>([&counter, totalToCullNumber]() {
				//	return counter >= totalToCullNumber;
				//}));
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