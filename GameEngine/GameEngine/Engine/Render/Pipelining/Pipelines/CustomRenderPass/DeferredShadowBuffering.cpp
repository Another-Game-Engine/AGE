#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredShadowBuffering.hh>

#include <memory>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <SpacePartitioning/Ouptut/RenderLight.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <SpacePartitioning/Ouptut/RenderPainter.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>
#include <Configuration.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Render/OcclusionTools/DepthMapHandle.hpp>
#include <Render/OcclusionTools/DepthMap.hpp>
#include <Render/Pipelining/Pipelines/PipelineTools.hh>
#include <glm/gtc/matrix_transform.hpp>

#define DEFERRED_SHADING_SHADOW_BUFFERING_VERTEX "deferred_shading/deferred_shading_get_shadow_buffer.vp"
#define DEFERRED_SHADING_SHADOW_BUFFERING_FRAG "deferred_shading/deferred_shading_get_shadow_buffer.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_BUFFERING = 0,
		PROGRAM_NBR
	};

	DeferredShadowBuffering::DeferredShadowBuffering(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager) :
		FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
		// We dont want to take the skinned or transparent meshes
		_forbidden[AGE_SKINNED] = true;
		_forbidden[AGE_SEMI_TRANSPARENT] = true;
		_programs.resize(PROGRAM_NBR);
		auto confManager = GetEngine()->getInstance<ConfigurationManager>();
		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");
		// you have to set shader directory in configuration path
		AGE_ASSERT(shaderPath != nullptr);
		auto vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_SHADOW_BUFFERING_VERTEX;
		auto fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_SHADOW_BUFFERING_FRAG;
		_programs[PROGRAM_BUFFERING] = std::make_shared<Program>(Program(std::string("program_shadow_buffering"),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));
	}

	void DeferredShadowBuffering::renderPass(RenderPipeline const &, RenderLightList &lights, CameraInfos const &infos)
	{
		SCOPE_profile_gpu_i("DeferredShadowBuffering render pass");
		SCOPE_profile_cpu_i("RenderTimer", "DeferredShadowBuffering render pass");

		OpenGLState::glEnable(GL_CULL_FACE);
		OpenGLState::glCullFace(GL_BACK);
		OpenGLState::glDepthMask(GL_TRUE);
		OpenGLState::glDepthFunc(GL_LEQUAL);
		OpenGLState::glDisable(GL_BLEND);
		OpenGLState::glDisable(GL_STENCIL_TEST);
		OpenGLState::glEnable(GL_DEPTH_TEST);
		OpenGLState::glDepthMask(GL_TRUE);
		OpenGLState::glDepthFunc(GL_LESS);

		_programs[PROGRAM_BUFFERING]->use();

		// handle the number of sample
		if (_depthBuffers.size() < lights.spotLights.size()) {
			int count = lights.spotLights.size() - _depthBuffers.size();
			for (int index = 0; index < count; ++index) {
				_depthBuffers.push_back(createRenderPassOutput<Texture2D>(_frame_buffer.width(), _frame_buffer.height(), GL_DEPTH24_STENCIL8, true));
				_depthBuffers.back()->bind();
				_depthBuffers.back()->parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				_depthBuffers.back()->parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				_depthBuffers.back()->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				_depthBuffers.back()->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
		}
		else if (_depthBuffers.size() > lights.spotLights.size()) {
			int count = _depthBuffers.size() - lights.spotLights.size();
			for (int index = 0; index < count; ++index) {
				_depthBuffers.pop_back();
			}
		}
		// start to render to texture for each depth map
		auto it = _depthBuffers.begin();
		for (auto &spotLight : lights.spotLights)
		{
			SCOPE_profile_gpu_i("Spotlight pass");
			SCOPE_profile_cpu_i("RenderTimer", "Spotlight pass");

			glViewport(0, 0, _frame_buffer.width(), _frame_buffer.height());
			auto projection = glm::perspective(60.f, (float)_frame_buffer.width() / (float)_frame_buffer.height(), 0.1f, 1000.0f);
			spotLight.shadow_matrix = projection * glm::inverse(spotLight.light.transformation);
			_programs[PROGRAM_BUFFERING]->get_resource<Mat4>("light_matrix").set(spotLight.shadow_matrix);
			_frame_buffer.attachment(*(*it), GL_DEPTH_STENCIL_ATTACHMENT);
			glClear(GL_DEPTH_BUFFER_BIT);
			// draw for the spot light selected
			for (auto &meshPaint : spotLight.keys)
			{
				SCOPE_profile_gpu_i("Draw mesh");
				SCOPE_profile_cpu_i("RenderTimer", "Draw mesh");

				auto painter = _painterManager->get_painter(Key<Painter>::createKey(meshPaint.first));
				for (auto &mode : meshPaint.second.drawables)
				{
					if (renderModeCompatible(mode.renderMode))
					{
						painter->draw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING], mode.properties, mode.vertices);
					}
				}
			}
			spotLight.shadow_map = *it;
			++it;
		}
	}

}