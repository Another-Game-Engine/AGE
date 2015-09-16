#pragma once

#include <vector>
#include <string>
#include <memory>
#include <list>

namespace AGE
{
	struct DRBCameraDrawableList;

	class IRenderingPipeline
	{
	public:
		virtual ~IRenderingPipeline() {}
		virtual std::string const &name() const = 0;
		virtual IRenderingPipeline &render(const DRBCameraDrawableList &infos) = 0;
		virtual bool recompileShaders() = 0;
		virtual bool init() = 0;
		virtual bool isDebug() const = 0;
	};

}