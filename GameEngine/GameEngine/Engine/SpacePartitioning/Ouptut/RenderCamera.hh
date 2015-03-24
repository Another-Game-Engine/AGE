#pragma once

#include <vector>
#include <SpacePartitioning/Ouptut/RenderLight.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <Render/PipelineTypes.hpp>
#include <glm/glm.hpp>

namespace AGE
{
	struct CameraInfos
	{
		RenderType renderType;
		glm::mat4 view;
		glm::mat4 projection;
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
			container.cameras.clear();
			container.used = false;
		}
	};
}