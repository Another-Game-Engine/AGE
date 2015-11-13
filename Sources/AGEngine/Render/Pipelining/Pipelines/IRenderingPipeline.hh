#pragma once

#include <vector>
#include <string>
#include <memory>
#include <list>

namespace AGE
{
	struct DRBCameraDrawableList;
	class SpotlightRenderInfos;
	class CameraRenderInfos;

	class IRenderingPipeline
	{
	public:
		virtual ~IRenderingPipeline() {}
		virtual std::string const &name() const = 0;
		virtual IRenderingPipeline &render(const DRBCameraDrawableList &infos) = 0;
		virtual bool recompileShaders() = 0;
		virtual bool init() = 0;
		virtual bool isDebug() const = 0;

		inline SpotlightRenderInfos *getSpotlightRenderInfos() { return _spotlightRenderInfos; }
		inline CameraRenderInfos *getCameraRenderInfos() { return _cameraRenderInfos; }
	protected:
		SpotlightRenderInfos *_spotlightRenderInfos = nullptr;
		CameraRenderInfos *_cameraRenderInfos = nullptr;
	};

}