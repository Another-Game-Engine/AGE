#include "SpotlightRenderInfos.hpp"

#include "Utils/Debug.hpp"
#include "Threads/ThreadManager.hpp"

#include <Utils/Profiler.hpp>

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

	// Call that after render preparation
	void SpotlightRenderInfos::computeRenderInfos()
	{
		//SCOPE_profile_cpu_function("RenderTimer");

		//AGE_ASSERT(IsRenderThread());

		//AGE_ASSERT(_meshs.empty());
		//AGE_ASSERT(_skinnedMeshs.empty());
		//AGE_ASSERT(_cameras.empty());
		//AGE_ASSERT(_spotlights.empty());

		//BasicCommandGeneration::MeshShadowOutput *mesh = nullptr;
		//while (_cullingResults.try_dequeue(mesh))
		//{
		//	_meshs.push_back(mesh);
		//}

		//BasicCommandGeneration::SkinnedShadowOutput *skinned = nullptr;
		//while (_skinnedCullingResults.try_dequeue(skinned))
		//{
		//	_skinnedMeshs.push_back(skinned);
		//}

		//Camera camera;
		//while (_cameraResults.try_dequeue(camera))
		//{
		//	_cameras.push_back(camera);
		//}

		//Spotlight spotlight;
		//while (_spotlightResults.try_dequeue(spotlight))
		//{
		//	_spotlights.push_back(spotlight);
		//}
	}

	// Call that when the rendering is done
	void SpotlightRenderInfos::clearRenderInfos()
	{
		//SCOPE_profile_cpu_function("RenderTimer");

		//AGE_ASSERT(IsRenderThread());

		//for (auto &e : _meshs)
		//{
		//	e->reset();
		//	BasicCommandGeneration::MeshShadowOutput::RecycleOutput(e);
		//}
		//_meshs.clear();

		//for (auto &e : _skinnedMeshs)
		//{
		//	e->reset();
		//	BasicCommandGeneration::SkinnedShadowOutput::RecycleOutput(e);
		//}

		//_skinnedMeshs.clear();

		//_cameras.clear();

		//_spotlights.clear();
	}
}