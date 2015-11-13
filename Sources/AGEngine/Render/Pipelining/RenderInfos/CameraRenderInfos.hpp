#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <Utils/Containers/LFQueue.hpp>

#include "Render\Pipelining\Prepare\MeshBufferingPrepare.hpp"

namespace AGE
{
	struct DRBCameraDrawableList;

	class CameraRenderInfos
	{
	public:
		typedef BasicCommandGeneration::MeshAndMaterialOutput MeshOutput;
		typedef BasicCommandGeneration::SkinnedMeshAndMaterialOutput SkinnedOutput;

		struct Camera
		{
			glm::mat4 projection;
			glm::mat4 view;
		};

		struct MeshInfos
		{
			MeshOutput* meshs;
			SkinnedOutput* skinnedMeshs;
		};

		// we have to put that in pool !!!
		class Output
		{
		public:
			const MeshInfos &setCameraInfos(const glm::mat4 &projection, const glm::mat4 &view);
			const std::vector<MeshInfos> &getMeshs() const { return _meshs; }
			const std::vector<Camera> &getCameras() const { return _cameras; }
		private:
			std::vector<MeshInfos> _meshs;
			std::vector<Camera> _cameras;
		};

		Output createOutput();

		CameraRenderInfos();

		const std::vector<Camera> &getCameras() const;
		const std::vector<MeshInfos> &getMeshs() const;
		
		// Call that after render preparation
		void computeRenderInfos(const DRBCameraDrawableList &infos);
		
		// Call that when the rendering is done
		void clearRenderInfos(const DRBCameraDrawableList &infos);

	private:
		LFQueue<BasicCommandGeneration::MeshShadowOutput*>          _cullingResults;
		LFQueue<BasicCommandGeneration::SkinnedShadowOutput*>       _skinnedCullingResults;

		std::vector<MeshInfos> _meshs;
		std::vector<Camera> _cameras;
	};
}