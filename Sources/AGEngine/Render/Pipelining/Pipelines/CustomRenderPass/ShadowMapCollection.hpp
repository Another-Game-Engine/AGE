#pragma once

#include "Render\Pipelining\Pipelines\PipelineTools.hh"
#include "Render\Textures\Texture2D.hh"
namespace AGE
{
	class ShadowMapCollection
	{
	public:
		static std::shared_ptr<Texture2D> getDepthBuffer(std::size_t index, int width, int height)
		{
			while (index >= _depthBuffers.size())
			{
				_depthBuffers.push_back(createRenderPassOutput<Texture2D>(width, height, GL_DEPTH24_STENCIL8, true));
				_depthBuffers.back()->bind();
				_depthBuffers.back()->parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				_depthBuffers.back()->parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				_depthBuffers.back()->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				_depthBuffers.back()->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				_depthBuffers.back()->parameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			}
			return _depthBuffers[index];
		}
	private:
		static std::vector<std::shared_ptr<Texture2D>> _depthBuffers;
	};
}