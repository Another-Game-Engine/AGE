#include <Render/Pipelining/Pipelines/CustomPipeline/DeferredShading.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredBasicBuffering.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredPointLightning.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredSpotLightning.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredDirectionalLightning.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredMerging.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredShadowBuffering.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredSkyBox.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredOnScreen.hh>
#include <Render/Pipelining/Pipelines/PipelineTools.hh>
#include <Configuration.hpp>

namespace AGE
{

	DeferredShading::DeferredShading(glm::uvec2 const &screen_size, std::shared_ptr<PaintingManager> const &painter_manager) :
		ARenderingPipeline(std::string("deferred shading"), painter_manager)
	{
		_diffuse = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA8, true);
		_diffuse->parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		_diffuse->parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		_normal = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA8, true);
		_specular = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA8, true);
		_depthStencil = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_DEPTH24_STENCIL8, true);
		// RGB = light color, A = specular power
		_lightAccumulation = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA16F, true);
		_lightAccumulation->parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		_lightAccumulation->parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		_shinyAccumulation = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA16F, true);
		_shinyAccumulation->parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		_shinyAccumulation->parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// We create the render pass
		std::shared_ptr<DeferredSkyBox> skybox = std::make_shared<DeferredSkyBox>(screen_size, _painter_manager, _diffuse, _depthStencil);
		std::shared_ptr<DeferredBasicBuffering> basicBuffering = std::make_shared<DeferredBasicBuffering>(screen_size, _painter_manager, _diffuse, _normal, _specular, _depthStencil);
		std::shared_ptr<DeferredSpotLightning> spotLightning = std::make_shared<DeferredSpotLightning>(screen_size, _painter_manager, _normal, _depthStencil, _specular, _lightAccumulation, _shinyAccumulation);
		std::shared_ptr<DeferredShadowBuffering> shadowBuffering = std::make_shared<DeferredShadowBuffering>(glm::uvec2(RESOLUTION_SHADOW_X, RESOLUTION_SHADOW_Y), _painter_manager);
		std::shared_ptr<DeferredPointLightning> pointLightning = std::make_shared<DeferredPointLightning>(screen_size, _painter_manager, _normal, _depthStencil, _specular, _lightAccumulation, _shinyAccumulation);
		std::shared_ptr<DeferredDirectionalLightning> directionalLightning = std::make_shared<DeferredDirectionalLightning>(screen_size, _painter_manager, _normal, _depthStencil, _specular, _lightAccumulation, _shinyAccumulation);
		_deferredMerging = std::make_shared<DeferredMerging>(screen_size, _painter_manager, _diffuse, _lightAccumulation, _shinyAccumulation);
		std::shared_ptr<DeferredOnScreen> deferredOnScreen = std::make_shared<DeferredOnScreen>(screen_size, _painter_manager, _diffuse);

		// The entry point is the basic buffering pass
		setAmbient(glm::vec3(0.2f));
		_rendering_list.emplace_back(shadowBuffering);
		_rendering_list.emplace_back(skybox);
		_rendering_list.emplace_back(basicBuffering);
		_rendering_list.emplace_back(directionalLightning);
		_rendering_list.emplace_back(spotLightning);
		_rendering_list.emplace_back(pointLightning);
		_rendering_list.emplace_back(_deferredMerging);
		_rendering_list.emplace_back(skybox);
		_rendering_list.emplace_back(deferredOnScreen);
	}

	DeferredShading::DeferredShading(DeferredShading &&move) :
		ARenderingPipeline(std::move(move))
	{

	}

	void DeferredShading::setAmbient(glm::vec3 const &ambient)
	{
		_deferredMerging->setAmbient(ambient);
	}

}