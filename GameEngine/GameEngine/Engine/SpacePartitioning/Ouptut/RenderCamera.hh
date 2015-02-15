#pragma once

#include <vector>
#include <SpacePartitioning/Ouptut/RenderLight.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <glm/glm.hpp>

namespace AGE
{

	struct CameraInfos
	{
		glm::mat4 view;
		glm::mat4 projection;
	};

	struct RenderCamera
	{
		CameraInfos camInfos;
		RenderLightList lights;
		std::vector<RenderPipeline> pipelines;
	};

}