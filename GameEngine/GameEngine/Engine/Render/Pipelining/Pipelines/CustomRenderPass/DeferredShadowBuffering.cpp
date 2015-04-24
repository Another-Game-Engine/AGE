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

	void DeferredShadowBuffering::renderPass(RenderPipeline const &, RenderLightList const &lights, CameraInfos const &infos)
	{
		OpenGLState::glEnable(GL_CULL_FACE);
		OpenGLState::glCullFace(GL_BACK);
		OpenGLState::glDepthMask(GL_TRUE);
		OpenGLState::glDepthFunc(GL_LEQUAL);
		OpenGLState::glDisable(GL_BLEND);
		OpenGLState::glDisable(GL_STENCIL_TEST);
		OpenGLState::glEnable(GL_DEPTH_TEST);

		_programs[PROGRAM_BUFFERING]->use();

		// handle the number of sample
		if (_depthBuffers.size() < lights.spotLights.size()) {
			int count = lights.spotLights.size() - _depthBuffers.size();
			for (int index = 0; index < count; ++index) {
				_depthBuffers.push_back(createRenderPassOutput<Texture2D>(RESOLUTION_SHADOW_X, RESOLUTION_SHADOW_Y, GL_DEPTH24_STENCIL8, false));
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
		for (auto &spotLight : lights.spotLights) {
			//glViewport(0, 0, RESOLUTION_SHADOW_X, RESOLUTION_SHADOW_Y);
			*_programs[PROGRAM_BUFFERING]->get_resource<Mat4>("projection_matrix") = glm::perspective(60.0f, (float)RESOLUTION_SHADOW_X / (float)RESOLUTION_SHADOW_Y, 0.1f, 2000.0f);
			*_programs[PROGRAM_BUFFERING]->get_resource<Mat4>("view_matrix") = infos.view;// spotLight.light.transformation;
			_frame_buffer.attachment(*(*it), GL_DEPTH_STENCIL_ATTACHMENT);
			glClear(GL_DEPTH_BUFFER_BIT);
			// draw for the spot light selected
			for (auto &meshPaint : spotLight.keys)
			{
				auto painter = _painterManager->get_painter(Key<Painter>::createKey(meshPaint.first));
				for (auto &mode : meshPaint.second.drawables)
				{
					if (renderModeCompatible(mode.renderMode))
					{
						painter->draw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING], mode.properties, mode.vertices);
					}
				}
			}
			++it;
			glFlush();
			glFlush();
		}
	}

}