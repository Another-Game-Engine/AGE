#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>

namespace AGE
{
	class Texture2D;
	class Program;

	class DeferredPointLightning : public FrameBufferRender
	{
	public:
		DeferredPointLightning(std::shared_ptr<PaintingManager> painterManager,
							std::shared_ptr<Texture2D> normal,
							std::shared_ptr<Texture2D> depth,
							std::shared_ptr<Texture2D> lightAccumulation);
		virtual ~DeferredPointLightning() = default;

	private:
		std::shared_ptr<Texture2D> _normalInput;
		std::shared_ptr<Texture2D> _depthInput;
	};
}

