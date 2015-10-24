#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <Utils/Containers/LFQueue.hpp>

#include "Render\Pipelining\Prepare\MeshBufferingPrepare.hpp"

namespace AGE
{
	/*
	Contains all infos needed by spotlight render pass
	*/

	class SpotlightRenderInfos
	{
	public:
		struct Camera
		{
			glm::mat4 projection;
			glm::mat4 view;
		};

		struct Spotlight
		{

		};

		SpotlightRenderInfos();

		LFQueue<BasicCommandGeneration::MeshShadowOutput*>* getMeshResultQueue();
		LFQueue<BasicCommandGeneration::SkinnedShadowOutput*>* getSkinnedResultQueue();
		const std::vector<Camera> &getCameras() const;
		const std::vector<Spotlight> &getSpotlights() const;
		const std::vector<BasicCommandGeneration::MeshShadowOutput*> &getMeshs() const;
		const std::vector<BasicCommandGeneration::SkinnedShadowOutput*> getSkinnedMeshs() const;
		void setCameraInfos(const glm::mat4 &projection, const glm::mat4 &view);
		
		// Call that after render preparation
		void computeRenderInfos();
		
		// Call that when the rendering is done
		void clearRenderInfos();

		typedef BasicCommandGeneration::MeshShadowOutput MeshOutput;
		typedef BasicCommandGeneration::SkinnedShadowOutput SkinnedOutput;

	private:
		LFQueue<BasicCommandGeneration::MeshShadowOutput*>          _cullingResults;
		LFQueue<BasicCommandGeneration::SkinnedShadowOutput*>       _skinnedCullingResults;
		LFQueue<Camera>												_cameraResults;
		LFQueue<Spotlight>											_spotlightResults;

		std::vector<BasicCommandGeneration::MeshShadowOutput*>      _meshs;
		std::vector<BasicCommandGeneration::SkinnedShadowOutput*>   _skinnedMeshs;
		std::vector<Camera>                                         _cameras;
		std::vector<Spotlight>                                      _spotlights;
	};
}