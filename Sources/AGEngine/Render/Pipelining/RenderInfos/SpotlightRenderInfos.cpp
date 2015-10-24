#include "SpotlightRenderInfos.hpp"

#include "Utils/Debug.hpp"
#include "Threads/ThreadManager.hpp"

namespace AGE
{
	SpotlightRenderInfos::SpotlightRenderInfos()
	{

	}

	LFQueue<BasicCommandGeneration::MeshShadowOutput*>* SpotlightRenderInfos::getMeshResultQueue()
	{
		AGE_ASSERT(IsMainThread());
		return &_cullingResults;
	}

	LFQueue<BasicCommandGeneration::SkinnedShadowOutput*>* SpotlightRenderInfos::getSkinnedResultQueue()
	{
		AGE_ASSERT(IsMainThread());
		return &_skinnedCullingResults;
	}

	const std::vector<SpotlightRenderInfos::Camera> &SpotlightRenderInfos::getCameras() const
	{
		AGE_ASSERT(IsRenderThread());
		return _cameras;
	}

	const std::vector<SpotlightRenderInfos::Spotlight> &SpotlightRenderInfos::getSpotlights() const
	{
		AGE_ASSERT(IsRenderThread());
		return _spotlights;
	}

	const std::vector<BasicCommandGeneration::MeshShadowOutput*> &SpotlightRenderInfos::getMeshs() const
	{
		AGE_ASSERT(IsRenderThread());
		return _meshs;
	}

	const std::vector<BasicCommandGeneration::SkinnedShadowOutput*> SpotlightRenderInfos::getSkinnedMeshs() const
	{
		AGE_ASSERT(IsRenderThread());
		return _skinnedMeshs;
	}

	void SpotlightRenderInfos::setCameraInfos(const glm::mat4 &projection, const glm::mat4 &view)
	{
		AGE_ASSERT(IsMainThread());
		Camera camera;
		camera.projection = projection;
		camera.view = view;
		_cameraResults.enqueue(camera);
	}

	// Call that after render preparation
	void SpotlightRenderInfos::computeRenderInfos()
	{
		AGE_ASSERT(IsRenderThread());

		AGE_ASSERT(_meshs.empty());
		AGE_ASSERT(_skinnedMeshs.empty());
		AGE_ASSERT(_cameras.empty());
		AGE_ASSERT(_spotlights.empty());

		BasicCommandGeneration::MeshShadowOutput *mesh = nullptr;
		while (_cullingResults.try_dequeue(mesh))
		{
			_meshs.push_back(mesh);
		}

		BasicCommandGeneration::SkinnedShadowOutput *skinned = nullptr;
		while (_skinnedCullingResults.try_dequeue(skinned))
		{
			_skinnedMeshs.push_back(skinned);
		}

		Camera camera;
		while (_cameraResults.try_dequeue(camera))
		{
			_cameras.push_back(camera);
		}

		Spotlight spotlight;
		while (_spotlightResults.try_dequeue(spotlight))
		{
			_spotlights.push_back(spotlight);
		}
	}

	// Call that when the rendering is done
	void SpotlightRenderInfos::clearRenderInfos()
	{
		AGE_ASSERT(IsRenderThread());

		for (auto &e : _meshs)
			BasicCommandGeneration::MeshShadowOutput::RecycleOutput(e);
		_meshs.clear();

		for (auto &e : _skinnedMeshs)
			BasicCommandGeneration::SkinnedShadowOutput::RecycleOutput(e);
		_skinnedMeshs.clear();

		_cameras.clear();

		_spotlights.clear();
	}
}