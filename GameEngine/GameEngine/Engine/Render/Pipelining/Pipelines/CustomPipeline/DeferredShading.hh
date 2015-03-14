#pragma once

#include <Utils/Debug.hpp>
#include <Utils/OpenGL.hh>
#include <Render/Pipelining/Pipelines/ARenderingPipeline.hh>
#include <glm/glm.hpp>
#include <Render/Textures/Texture2D.hh>
#include <memory>

namespace AGE
{

	class DeferredShading : public ARenderingPipeline
	{
	public:
		enum Step
		{
			BUFFERING = 0,
			LIGHTNING,
			MERGING,
			TOTAL
		};

	public:
		DeferredShading(glm::uvec2 const &screen_size, std::shared_ptr<PaintingManager> const &painter_manager);
		DeferredShading(DeferredShading &&move);
		virtual ~DeferredShading() {};
		DeferredShading(DeferredShading const &) = delete;
		DeferredShading &operator=(DeferredShading const &) = delete;

	public:
		virtual IRenderingPipeline &render(RenderPipeline const &pipeline, RenderLightList const &lights, CameraInfos const &camera) override final;

	private:
		std::shared_ptr<Texture2D> _diffuseTexture;
		std::shared_ptr<Texture2D> _normalTexture;
		std::shared_ptr<Texture2D> _specularTexture;
		std::shared_ptr<Texture2D> _depthTexture;

		std::shared_ptr<Texture2D> _lightAccumulationTexture;

		RenderLightList const *_lights;
	};


	template <typename OUTPUT, typename RENDER, typename ...Params>
	std::shared_ptr<OUTPUT> addRenderPassOutput(std::shared_ptr<IRendering> const &render, GLenum attachment, Params... params)
	{
		auto const &output = std::make_shared<OUTPUT>();
		auto error = output->init(std::forward<Params>(params)...);
		AGE_ASSERT(error != false && "Texture generation error.");
		std::static_pointer_cast<RENDER>(render)->push_storage_output<OUTPUT>(attachment, output);
		return output;
	}

}