#include <Render/Pipelining/Pipelines/CustomPipeline/DebugDeferredShading.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredBasicBuffering.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredPointLightning.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredSpotLightning.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredDirectionalLightning.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredMerging.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredShadowBuffering.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredSkyBox.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredOnScreen.hh>

#include <Render/Pipelining/Pipelines/CustomRenderPass/DebugDrawLines.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DebugLightBillboards.hh>

#include <Render/Pipelining/Pipelines/PipelineTools.hh>
#include <Configuration.hpp>

namespace AGE
{

	DebugDeferredShading::DebugDeferredShading(glm::uvec2 const &screen_size, std::shared_ptr<PaintingManager> const &painter_manager) :
		ARenderingPipeline(std::string("deferred shading"), painter_manager)
	{
		_albedo = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA32F, true);
		_diffuse = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA32F, true);
		_normal = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA8, true);
		_specular = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA8, true);
		_depthStencil = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_DEPTH24_STENCIL8, true);
		// RGB = light color, A = specular power
		_lightAccumulation = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA32F, true);
		_shinyAccumulation = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA32F, true);

		// We create the render pass
		_deferredSkybox = std::make_shared<DeferredSkyBox>(screen_size, _painter_manager, _diffuse, _depthStencil, _lightAccumulation);
		auto basicBuffering = std::make_shared<DeferredBasicBuffering>(screen_size, _painter_manager, _diffuse, _normal, _specular, _depthStencil);
		auto spotLightning = std::make_shared<DeferredSpotLightning>(screen_size, _painter_manager, _normal, _depthStencil, _specular, _lightAccumulation, _shinyAccumulation);
		auto shadowBuffering = std::make_shared<DeferredShadowBuffering>(glm::uvec2(RESOLUTION_SHADOW_X, RESOLUTION_SHADOW_Y), _painter_manager);
		auto pointLightning = std::make_shared<DeferredPointLightning>(screen_size, _painter_manager, _normal, _depthStencil, _specular, _lightAccumulation, _shinyAccumulation);
		auto directionalLightning = std::make_shared<DeferredDirectionalLightning>(screen_size, _painter_manager, _normal, _depthStencil, _specular, _lightAccumulation, _shinyAccumulation);
		_deferredMerging = std::make_shared<DeferredMerging>(screen_size, _painter_manager, _diffuse, _lightAccumulation, _shinyAccumulation, _diffuse);
		std::shared_ptr<DeferredOnScreen> deferredOnScreen = std::make_shared<DeferredOnScreen>(screen_size, _painter_manager, _diffuse);

		// Debug render passes
		std::shared_ptr<DebugDrawLines> debugDrawLines = std::make_shared<DebugDrawLines>(screen_size, _painter_manager, _diffuse, _depthStencil);
		std::shared_ptr<DebugLightBillboards> debugLightBillboards = std::make_shared<DebugLightBillboards>(screen_size, _painter_manager, _diffuse, _depthStencil);

		// The entry point is the basic buffering pass
		setAmbient(glm::vec3(0.2f));
		setSkyboxLighting(glm::vec3(0.8f));
		_rendering_list.emplace_back(shadowBuffering);
		_rendering_list.emplace_back(basicBuffering);
		_rendering_list.emplace_back(directionalLightning);
		_rendering_list.emplace_back(spotLightning);
		_rendering_list.emplace_back(pointLightning);
		_rendering_list.emplace_back(_deferredMerging);
		_rendering_list.emplace_back(_deferredSkybox);
		_rendering_list.emplace_back(debugLightBillboards);
		_rendering_list.emplace_back(debugDrawLines);
		_rendering_list.emplace_back(deferredOnScreen);
	}

	DebugDeferredShading::DebugDeferredShading(DebugDeferredShading &&move) :
		ARenderingPipeline(std::move(move))
	{

	}

	void DebugDeferredShading::setAmbient(glm::vec3 const &ambient)
	{
		_deferredMerging->setAmbient(ambient);
	}

	void DebugDeferredShading::setSkyboxLighting(glm::vec3 const &lighting)
	{
		_deferredSkybox->setSkyboxLighting(lighting);
	}

}