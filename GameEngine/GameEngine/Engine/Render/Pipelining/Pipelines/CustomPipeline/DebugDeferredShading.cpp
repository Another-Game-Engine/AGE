#include <Render/Pipelining/Pipelines/CustomPipeline/DebugDeferredShading.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredBasicBuffering.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredDebugBuffering.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredPointLightning.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredSpotLightning.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredDirectionalLightning.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredMerging.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredMergingDebug.hh>
#include <Render/Pipelining/Pipelines/PipelineTools.hh>

namespace AGE
{

	DebugDeferredShading::DebugDeferredShading(glm::uvec2 const &screen_size, std::shared_ptr<PaintingManager> const &painter_manager) :
		ARenderingPipeline(std::string("deferred shading"), painter_manager)
	{
		_diffuse = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA8, true);
		_normal = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA8, true);
		_specular = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA8, true);
		_depthStencil = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_DEPTH24_STENCIL8, true);
		_debugLights = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA8, true);
		// RGB = light color, A = specular power
		_lightAccumulation = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA8, true);

		// We create the render pass
		std::shared_ptr<DeferredBasicBuffering> basicBuffering = std::make_shared<DeferredBasicBuffering>(_painter_manager, _diffuse, _normal, _specular, _depthStencil);
		std::shared_ptr<DeferredDebugBuffering> debugBuffering = std::make_shared<DeferredDebugBuffering>(_painter_manager, _debugLights, _depthStencil);
		std::shared_ptr<DeferredPointLightning> pointLightning = std::make_shared<DeferredPointLightning>(_painter_manager, _normal, _depthStencil, _lightAccumulation);
		std::shared_ptr<DeferredSpotLightning> spotLightning = std::make_shared<DeferredSpotLightning>(_painter_manager, _normal, _depthStencil, _lightAccumulation);
		std::shared_ptr<DeferredDirectionalLightning> directionalLightning = std::make_shared<DeferredDirectionalLightning>(_painter_manager, _normal, _depthStencil, _lightAccumulation);

		_deferredMerging = std::make_shared<DeferredMerging>(_painter_manager, _diffuse, _specular, _lightAccumulation);
		std::shared_ptr<DeferredMergingDebug> debugMerging = std::make_shared<DeferredMergingDebug>(_painter_manager, _debugLights);

		// The entry point is the basic buffering pass
		_rendering_list.emplace_back(basicBuffering);
		// We link the entry point with the other pass
		basicBuffering->setNextPass(debugBuffering);
		debugBuffering->setNextPass(spotLightning);
		spotLightning->setNextPass(directionalLightning);
		directionalLightning->setNextPass(pointLightning);
		pointLightning->setNextPass(_deferredMerging);
		_deferredMerging->setNextPass(debugMerging);

		setAmbient(glm::vec3(0.2f));
	}

	DebugDeferredShading::DebugDeferredShading(DebugDeferredShading &&move) :
		ARenderingPipeline(std::move(move))
	{

	}

	void DebugDeferredShading::setAmbient(glm::vec3 const &ambient)
	{
		_deferredMerging->setAmbient(ambient);
	}

}