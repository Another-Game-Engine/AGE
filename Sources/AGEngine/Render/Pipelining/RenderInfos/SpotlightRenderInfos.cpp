#include "SpotlightRenderInfos.hpp"

#include "Utils/Debug.hpp"
#include "Threads/ThreadManager.hpp"

#include <Utils/Profiler.hpp>

#include <Graphic\DRBCameraDrawableList.hpp>

namespace AGE
{
	SpotlightRenderInfos::SpotlightRenderInfos()
	{
	}

	void SpotlightRenderInfos::Output::setCameraInfos(const glm::mat4 &projection, const glm::mat4 &view)
	{
		AGE_ASSERT(IsMainThread());
		
		Camera camera;
		camera.projection = projection;
		camera.view = view;
		_cameras.push_back(camera);
	}
	
	const SpotlightRenderInfos::Spotlight &SpotlightRenderInfos::Output::setSpotlightInfos(
		const glm::vec3 &position,
		const glm::vec3 &attenuation,
		const glm::vec3 &direction,
		const glm::vec3 &color,
		const glm::mat4 &matrix,
		const float &cutOff,
		const float &exponent)
	{
		AGE_ASSERT(IsMainThread());
		
		Spotlight spot;
		spot.position = position;
		spot.attenuation = attenuation;
		spot.direction = direction;
		spot.color = color;
		spot.matrix = matrix;
		spot.cutOff = cutOff;
		spot.exponent = exponent;
		spot.meshs = SpotlightRenderInfos::MeshOutput::GetNewOutput();
		spot.skinnedMeshs = SpotlightRenderInfos::SkinnedOutput::GetNewOutput();
		_spots.push_back(spot);
		return _spots.back();
	}

	SpotlightRenderInfos::Output SpotlightRenderInfos::createOutput()
	{
		Output output;
		return output;
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

	// Call that after render preparation
	void SpotlightRenderInfos::computeRenderInfos(const DRBCameraDrawableList &infos)
	{
		SCOPE_profile_cpu_function("RenderTimer");

		AGE_ASSERT(IsRenderThread());

		AGE_ASSERT(_cameras.empty());
		AGE_ASSERT(_spotlights.empty());

		for (auto &camera : infos.spotlightsOutput.getCameras())
		{
			_cameras.push_back(camera);
		}

		for (auto &spot : infos.spotlightsOutput.getSpots())
		{
			_spotlights.push_back(spot);
		}
	}

	// Call that when the rendering is done
	void SpotlightRenderInfos::clearRenderInfos(const DRBCameraDrawableList &infos)
	{
		SCOPE_profile_cpu_function("RenderTimer");

		AGE_ASSERT(IsRenderThread());

		for (auto &e : _spotlights)
		{
			if (e.meshs)
			{
				e.meshs->reset();
				BasicCommandGeneration::MeshShadowOutput::RecycleOutput(e.meshs);
			}
			if (e.skinnedMeshs)
			{
				e.skinnedMeshs->reset();
				BasicCommandGeneration::SkinnedShadowOutput::RecycleOutput(e.skinnedMeshs);
			}
		}
		_cameras.clear();
		_spotlights.clear();
	}
}