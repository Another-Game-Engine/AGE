#pragma once

#include <functional>
#include <Utils/Containers/Vector.hpp>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <AssetManagement/Instance/MaterialInstance.hh>

namespace AGE
{
	class Engine;

	namespace Commands
	{
		namespace ToRender
		{
			struct Flush{
				bool isRenderFrame;
				Flush(bool isRenderFrame)
					: isRenderFrame(isRenderFrame)
				{}
			};
		}
	}
}