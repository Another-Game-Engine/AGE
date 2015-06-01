#pragma once

#include <vector>
#include <SpacePartitioning/Ouptut/RenderLight.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <Components/CameraComponent.hpp>
#include <Render/PipelineTypes.hpp>
#include <glm/glm.hpp>
#include <Utils/Profiler.hpp>

namespace AGE
{
	struct CameraInfos
	{
		glm::mat4 view;
		CameraData data;
	};

	struct RenderCamera
	{
		CameraInfos camInfos;
		RenderLightList lights;
		RenderPipeline pipeline;
	};

	struct RenderCameraListContainer
	{
		std::vector<RenderCamera> cameras;
		std::atomic_bool used;

		RenderCameraListContainer()
		{
			used = false;
		}
	};

	struct RenderCameraListContainerHandle
	{
		RenderCameraListContainer &container;

		RenderCameraListContainerHandle() = delete;
		RenderCameraListContainerHandle(RenderCameraListContainer &ref)
			: container(ref)
		{
			container.used = true;
		}
		~RenderCameraListContainerHandle()
		{
			container.used = false;
		}
	};
}