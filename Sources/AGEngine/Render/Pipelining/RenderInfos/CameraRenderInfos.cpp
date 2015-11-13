#include "CameraRenderInfos.hpp"

#include "Utils/Debug.hpp"
#include "Threads/ThreadManager.hpp"

#include <Utils/Profiler.hpp>
#include <Graphic\DRBCameraDrawableList.hpp>

namespace AGE
{
	CameraRenderInfos::CameraRenderInfos()
	{
	}

	const CameraRenderInfos::MeshInfos &CameraRenderInfos::Output::setCameraInfos(const glm::mat4 &projection, const glm::mat4 &view)
	{
		AGE_ASSERT(IsMainThread());
		
		Camera camera;
		camera.projection = projection;
		camera.view = view;
		_cameras.push_back(camera);

		MeshInfos meshs;
		meshs.meshs = CameraRenderInfos::MeshOutput::GetNewOutput();
		meshs.skinnedMeshs = CameraRenderInfos::SkinnedOutput::GetNewOutput();
		_meshs.push_back(meshs);
		return _meshs.back();
	}
	
	CameraRenderInfos::Output CameraRenderInfos::createOutput()
	{
		Output output;
		return output;
	}

	const std::vector<CameraRenderInfos::Camera> &CameraRenderInfos::getCameras() const
	{
		AGE_ASSERT(IsRenderThread());
		return _cameras;
	}

	const std::vector<CameraRenderInfos::MeshInfos> &CameraRenderInfos::getMeshs() const
	{
		AGE_ASSERT(IsRenderThread());
		return _meshs;
	}

	// Call that after render preparation
	void CameraRenderInfos::computeRenderInfos(const DRBCameraDrawableList &infos)
	{
		SCOPE_profile_cpu_function("RenderTimer");

		AGE_ASSERT(IsRenderThread());

		AGE_ASSERT(_cameras.empty());
		AGE_ASSERT(_meshs.empty());

		for (auto &camera : infos.camerasOutput.getCameras())
		{
			_cameras.push_back(camera);
		}

		for (auto &meshs : infos.camerasOutput.getMeshs())
		{
			_meshs.push_back(meshs);
		}
	}

	// Call that when the rendering is done
	void CameraRenderInfos::clearRenderInfos(const DRBCameraDrawableList &infos)
	{
		SCOPE_profile_cpu_function("RenderTimer");

		AGE_ASSERT(IsRenderThread());

		for (auto &e : _meshs)
		{
			if (e.meshs)
			{
				e.meshs->reset();
				MeshOutput::RecycleOutput(e.meshs);
			}
			if (e.skinnedMeshs)
			{
				e.skinnedMeshs->reset();
				SkinnedOutput::RecycleOutput(e.skinnedMeshs);
			}
		}
		_cameras.clear();
		_meshs.clear();
	}
}