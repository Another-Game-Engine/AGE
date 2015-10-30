#pragma once

#include <System/System.h>
#include <Core/EntityFilter.hpp>
#include <BFC/BFCCuller.hpp>
#include <BFC/BFCFrustumCuller.hpp>

namespace AGE
{
	struct DRBCameraDrawableList;

	class RenderCameraSystem : public System<RenderCameraSystem>
	{
	public:
		RenderCameraSystem() = delete;
		RenderCameraSystem(AScene *scene);
		~RenderCameraSystem() = default;
		void drawDebugLines(bool activated);
		inline bool &enableCulling() { return _cullingEnabled; }
	private:
		EntityFilter _cameras;
		EntityFilter _spotLights;
		EntityFilter _directionnalLights;
		EntityFilter _pointLights;
		bool         _drawDebugLines;
		bool         _cullingEnabled;

		std::atomic_uint64_t _spotCounter;
		std::vector<std::shared_ptr<DRBCameraDrawableList>> _camerasDrawLists;
		std::list<std::atomic_size_t> _cameraCounters;
		std::list<BFCCuller<BFCFrustumCuller>> _frustumCullers;

		virtual bool initialize();
		virtual void mainUpdate(float time);
	};
}
