#pragma once

#include <Utils/Debug.hpp>
#include <Utils/OpenGL.hh>
#include <Render/Pipelining/Pipelines/ARenderingPipeline.hh>
#include <SpacePartitioning/Ouptut/RenderLight.hh>
#include <glm/glm.hpp>
#include <Render/Textures/Texture2D.hh>
#include <memory>

namespace AGE
{

	class DeferredShading : public ARenderingPipeline
	{
	public:
		DeferredShading(glm::uvec2 const &screen_size, std::shared_ptr<PaintingManager> const &painter_manager);
		DeferredShading(DeferredShading &&move);
		virtual ~DeferredShading() {};
		DeferredShading(DeferredShading const &) = delete;
		DeferredShading &operator=(DeferredShading const &) = delete;

	public:
		virtual IRenderingPipeline &render(RenderPipeline const &pipeline, RenderLightList const &lights, CameraInfos const &camera) override final;

	private:
		std::shared_ptr<Texture2D> _depthStencil;
		std::shared_ptr<Texture2D> _diffuse;
		std::shared_ptr<Texture2D> _normal;
		std::shared_ptr<Texture2D> _specular;
		std::shared_ptr<Texture2D> _lightAccumulation;
	};


	template <typename OUTPUT, typename ...Params>
	std::shared_ptr<OUTPUT> createRenderPassOutput(Params... params)
	{
		auto const &output = std::make_shared<OUTPUT>();
		auto error = output->init(std::forward<Params>(params)...);
		AGE_ASSERT(error != false && "Texture generation error.");
		return output;
	}

}