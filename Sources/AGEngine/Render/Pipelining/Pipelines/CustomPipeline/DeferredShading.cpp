#include <Render/Pipelining/Pipelines/CustomPipeline/DeferredShading.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredBasicBuffering.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredPointLightning.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredSpotLightning.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredDirectionalLightning.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredMerging.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredShadowBuffering.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredSkyBox.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredOnScreen.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DownSample.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/GaussianBlur.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DepthOfField.hh>
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredBloomMerge.hh>
#include <Render/Pipelining/Pipelines/PipelineTools.hh>
#include <Configuration.hpp>

namespace AGE
{

	DeferredShading::DeferredShading(glm::uvec2 const &screen_size, std::shared_ptr<PaintingManager> const &painter_manager) :
		ARenderingPipeline(std::string("deferred shading"), painter_manager)
	{
		_diffuse = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA16F, false);
		_diffuse->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		_diffuse->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		_normal = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA8, true);
		_normal->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		_normal->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		_specular = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA16F, true);
		_specular->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		_specular->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		_depthStencil = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_DEPTH24_STENCIL8, true);
		_depthStencil->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		_depthStencil->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// RGB = light color, A = specular power
		_lightAccumulation = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA16F, true);
		_lightAccumulation->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		_lightAccumulation->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		_shinyAccumulation = createRenderPassOutput<Texture2D>(screen_size.x, screen_size.y, GL_RGBA16F, true);
		_shinyAccumulation->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		_shinyAccumulation->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		_downSampled1 = createRenderPassOutput<Texture2D>(screen_size.x / 2, screen_size.y / 2, GL_RGBA16F, false);
		_downSampled1->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		_downSampled1->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		_downSampled2 = createRenderPassOutput<Texture2D>(screen_size.x / 4, screen_size.y / 4, GL_RGBA16F, false);
		_downSampled2->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		_downSampled2->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//		_downSampled3 = createRenderPassOutput<Texture2D>(screen_size.x / 8, screen_size.y / 8, GL_RGBA16F, false);
		//		_downSampled3->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//		_downSampled3->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		_blurTmp1 = createRenderPassOutput<Texture2D>(screen_size.x / 2, screen_size.y / 2, GL_RGBA16F, false);
		_blurTmp1->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		_blurTmp1->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		_blurTmp2 = createRenderPassOutput<Texture2D>(screen_size.x / 4, screen_size.y / 4, GL_RGBA16F, false);
		_blurTmp2->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		_blurTmp2->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		_blurTmp3 = createRenderPassOutput<Texture2D>(screen_size.x / 8, screen_size.y / 8, GL_RGBA16F, false);
//		_blurTmp3->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		_blurTmp3->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// We create the render pass
		_deferredSkybox = std::make_shared<DeferredSkyBox>(screen_size, _painter_manager, _diffuse, _depthStencil, _lightAccumulation);
		std::shared_ptr<DeferredBasicBuffering> basicBuffering = std::make_shared<DeferredBasicBuffering>(screen_size, _painter_manager, _diffuse, _normal, _specular, _depthStencil);
		std::shared_ptr<DeferredSpotLightning> spotLightning = std::make_shared<DeferredSpotLightning>(screen_size, _painter_manager, _normal, _depthStencil, _specular, _lightAccumulation, _shinyAccumulation);
		std::shared_ptr<DeferredShadowBuffering> shadowBuffering = std::make_shared<DeferredShadowBuffering>(glm::uvec2(RESOLUTION_SHADOW_X, RESOLUTION_SHADOW_Y), _painter_manager, this);
		std::shared_ptr<DeferredPointLightning> pointLightning = std::make_shared<DeferredPointLightning>(screen_size, _painter_manager, _normal, _depthStencil, _specular, _lightAccumulation, _shinyAccumulation);
		std::shared_ptr<DeferredDirectionalLightning> directionalLightning = std::make_shared<DeferredDirectionalLightning>(screen_size, _painter_manager, _normal, _depthStencil, _specular, _lightAccumulation, _shinyAccumulation);
		_deferredMerging = std::make_shared<DeferredMerging>(screen_size, _painter_manager, _diffuse, _lightAccumulation, _shinyAccumulation);

		std::shared_ptr<DownSample> downSample1 = std::make_shared<DownSample>(screen_size / glm::uvec2(2), _painter_manager, _diffuse, _downSampled1);
		std::shared_ptr<DownSample> downSample2 = std::make_shared<DownSample>(screen_size / glm::uvec2(4), _painter_manager, _downSampled1, _downSampled2);
//		std::shared_ptr<DownSample> downSample3 = std::make_shared<DownSample>(screen_size / glm::uvec2(8), _painter_manager, _downSampled2, _downSampled3);

		std::shared_ptr<GaussianBlur> depthHorizontalPass1 = std::make_shared<GaussianBlur>(screen_size / glm::uvec2(2), _painter_manager, _downSampled1, _blurTmp1, true);
		std::shared_ptr<GaussianBlur> depthVerticalPass1 = std::make_shared<GaussianBlur>(screen_size / glm::uvec2(2), _painter_manager, _blurTmp1, _downSampled1, false);

		std::shared_ptr<GaussianBlur> depthHorizontalPass2 = std::make_shared<GaussianBlur>(screen_size / glm::uvec2(4), _painter_manager, _downSampled2, _blurTmp2, true);
		std::shared_ptr<GaussianBlur> depthVerticalPass2 = std::make_shared<GaussianBlur>(screen_size / glm::uvec2(4), _painter_manager, _blurTmp2, _downSampled2, false);

//		std::shared_ptr<GaussianBlur> depthHorizontalPass3 = std::make_shared<GaussianBlur>(screen_size / glm::uvec2(8), _painter_manager, _downSampled3, _blurTmp3, true);
//		std::shared_ptr<GaussianBlur> depthVerticalPass3 = std::make_shared<GaussianBlur>(screen_size / glm::uvec2(8), _painter_manager, _blurTmp3, _downSampled3, false);

		std::shared_ptr<DepthOfField> depthOfField = std::make_shared<DepthOfField>(screen_size, _painter_manager, _depthStencil, _downSampled1, _downSampled2, _diffuse, _diffuse);

		std::shared_ptr<DeferredBloomMerge> bloomMerge = std::make_shared<DeferredBloomMerge>(screen_size, _painter_manager, _downSampled1, _downSampled2, _diffuse, _diffuse);

		std::shared_ptr<DeferredOnScreen> deferredOnScreen = std::make_shared<DeferredOnScreen>(screen_size, _painter_manager, _diffuse);

		// The entry point is the basic buffering pass
		setAmbient(glm::vec3(0.2f));
		setSkyboxLighting(glm::vec3(0.8f));
		_rendering_list.emplace_back(shadowBuffering);
		_rendering_list.emplace_back(basicBuffering);
		_rendering_list.emplace_back(directionalLightning);
		_rendering_list.emplace_back(_deferredSkybox);
		_rendering_list.emplace_back(spotLightning);
		_rendering_list.emplace_back(pointLightning);
		_rendering_list.emplace_back(_deferredMerging);
		_rendering_list.emplace_back(downSample1);
		_rendering_list.emplace_back(downSample2);
//		_rendering_list.emplace_back(downSample3);

		// Double blur
		_rendering_list.emplace_back(depthHorizontalPass1);
		_rendering_list.emplace_back(depthVerticalPass1);
		_rendering_list.emplace_back(depthHorizontalPass2);
		_rendering_list.emplace_back(depthVerticalPass2);
		//		_rendering_list.emplace_back(depthHorizontalPass3);
		//		_rendering_list.emplace_back(depthVerticalPass3);

		// Merge blur with clear image for depth of field
		_rendering_list.emplace_back(depthOfField);

		// Then finally merge for the bloom
		_rendering_list.emplace_back(bloomMerge);

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

	void DeferredShading::setSkyboxLighting(glm::vec3 const &lighting)
	{
		_deferredSkybox->setSkyboxLighting(lighting);
	}
	
	DeferredShading::~DeferredShading()
	{
	}

	bool DeferredShading::isDebug() const
	{
		return false;
	}

}