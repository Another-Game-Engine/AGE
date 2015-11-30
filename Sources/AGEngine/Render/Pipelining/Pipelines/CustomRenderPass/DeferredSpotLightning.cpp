#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredSpotLightning.hh>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <Render/ProgramResources/Types/Uniform/Vec3.hh>
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/ConfigurationManager.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Core/Engine.hh>

#include "Graphic\DRBCameraDrawableList.hpp"

#include "ShadowMapCollection.hpp"

#include <Render/Pipelining/Pipelines/IRenderingPipeline.hh>
#include <Render/Pipelining/RenderInfos/SpotlightRenderInfos.hpp>

#define DEFERRED_SHADING_SPOT_LIGHT_VERTEX "deferred_shading/deferred_shading_spot_light.vp"
#define DEFERRED_SHADING_SPOT_LIGHT_FRAG "deferred_shading/deferred_shading_spot_light.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_LIGHTNING,
		PROGRAM_NBR
	};

	DeferredSpotLightning::DeferredSpotLightning(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> normal,
		std::shared_ptr<Texture2D> depth,
		std::shared_ptr<Texture2D> specular,
		std::shared_ptr<Texture2D> lightAccumulation,
		std::shared_ptr<Texture2D> shinyAccumulation,
		IRenderingPipeline *pipeline) :
		FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
		_pipeline = pipeline;

		push_storage_output(GL_COLOR_ATTACHMENT0, lightAccumulation);
		push_storage_output(GL_COLOR_ATTACHMENT1, shinyAccumulation);
		push_storage_output(GL_DEPTH_STENCIL_ATTACHMENT, depth);

		_normalInput = normal;
		_depthInput = depth;
		_specularInput = specular;

		_programs.resize(PROGRAM_NBR);
		auto confManager = GetEngine()->getInstance<ConfigurationManager>();
		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");
		// you have to set shader directory in configuration path
		AGE_ASSERT(shaderPath != nullptr);
		std::string vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_SPOT_LIGHT_VERTEX;
		std::string fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_SPOT_LIGHT_FRAG;

		_programs[PROGRAM_LIGHTNING] = std::make_shared<Program>(Program(StringID("program_spot_light", 0xea31766bcb94cc9b),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));
		GetRenderThread()->getQuadGeometry(_quad, _quadPainter);

	}

	void DeferredSpotLightning::renderPass(const DRBCameraDrawableList &infos)
	{
		SCOPE_profile_gpu_i("DeferredSpotLightning render pass");
		SCOPE_profile_cpu_i("RenderTimer", "DeferredSpotLightning render pass");

		if (_pipeline->getSpotlightRenderInfos()->getCameras().empty())
			return;

		// Just one camera supported for the moment
		auto &camera = _pipeline->getSpotlightRenderInfos()->getCameras().front();

		glm::vec3 cameraPosition = -glm::transpose(glm::mat3(camera.view)) * glm::vec3(camera.view[3]);

		_programs[PROGRAM_LIGHTNING]->use();
		_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>(StringID("projection_matrix", 0x92b1e336c34a1224)).set(camera.projection);
		_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>(StringID("view_matrix", 0xd15d560e7965726c)).set(camera.view);
		_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>(StringID("normal_buffer", 0x313e2189c71f910d)).set(_normalInput);
		_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>(StringID("specular_buffer", 0x0824313afd644f03)).set(_specularInput);
		_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>(StringID("depth_buffer", 0x2a88a65798cfc925)).set(_depthInput);
		_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>(StringID("eye_pos", 0xe58566afddb7bc1f)).set(cameraPosition);

		OpenGLState::glDisable(GL_CULL_FACE);
		OpenGLState::glDisable(GL_DEPTH_TEST);
		OpenGLState::glEnable(GL_STENCIL_TEST);
		OpenGLState::glStencilFunc(GL_EQUAL, 0, 0xFF);
		OpenGLState::glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		// And we set the blend mode to additive
		OpenGLState::glEnable(GL_BLEND);
		OpenGLState::glBlendFunc(GL_ONE, GL_ONE);

		auto painter = _painterManager->get_painter(_quadPainter);
		int i = 0;
		auto w = _frame_buffer.width(); auto h = _frame_buffer.height();
		for (auto &spot : _pipeline->getSpotlightRenderInfos()->getSpotlights())
		{
			auto depth = ShadowMapCollection::getDepthBuffer(i++, w, h);

			_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>("shadow_map").set(depth);

			_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>(StringID("position_light", 0x514f03a54d8ceae9)).set(spot.position);
			_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>(StringID("attenuation_light", 0x344423c4b06b660c)).set(spot.attenuation);
			_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>(StringID("direction_light", 0xc2d48b1631dca809)).set(spot.direction);
			_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>(StringID("color_light", 0x7da5b3f55d350b6f)).set(spot.color);
			_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>(StringID("light_matrix", 0x9c8229a430a9c8a9)).set(spot.matrix);
			_programs[PROGRAM_LIGHTNING]->get_resource<Vec1>(StringID("spot_cut_off", 0x60934f6991e9b910)).set(spot.cutOff);
			_programs[PROGRAM_LIGHTNING]->get_resource<Vec1>(StringID("exponent_light", 0xef65e2e5c90f9125)).set(spot.exponent);

			painter->uniqueDrawBegin(_programs[PROGRAM_LIGHTNING]);
			painter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_LIGHTNING], _quad);
			painter->uniqueDrawEnd();
		}
	}
}
