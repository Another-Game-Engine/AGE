#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredDebugBuffering.hh>

#include <memory>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <SpacePartitioning/Ouptut/RenderLight.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <SpacePartitioning/Ouptut/RenderPainter.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>

#define DEFERRED_SHADING_DEBUG_LIGHT_VERTEX "../../Shaders/deferred_shading/deferred_shading_debug_light.vp"
#define DEFERRED_SHADING_DEBUG_LIGHT_FRAG "../../Shaders/deferred_shading/deferred_shading_debug_light.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_BUFFERING_LIGHT = 0,
		PROGRAM_NBR
	};

	DeferredDebugBuffering::DeferredDebugBuffering(std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> debugLights,
		std::shared_ptr<Texture2D> depth) :
		FrameBufferRender(painterManager)
	{
		Key<Painter> quadPainterKey;
		GetRenderThread()->getQuadGeometry(_quadVertices, quadPainterKey);
		_quadPainter = _painterManager->get_painter(quadPainterKey);

		// We dont want to take the skinned or transparent meshes
		_forbidden[AGE_SKINNED] = true;
		_forbidden[AGE_SEMI_TRANSPARENT] = true;

		push_storage_output(GL_COLOR_ATTACHMENT0, debugLights);
		push_storage_output(GL_DEPTH_STENCIL_ATTACHMENT, depth);

		_programs.resize(PROGRAM_NBR);

		_programs[PROGRAM_BUFFERING_LIGHT] = std::make_shared<Program>(Program(std::string("debug_lights"),
		{
			std::make_shared<UnitProg>(DEFERRED_SHADING_DEBUG_LIGHT_VERTEX, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(DEFERRED_SHADING_DEBUG_LIGHT_FRAG, GL_FRAGMENT_SHADER)
		}));
	}

	void DeferredDebugBuffering::renderPass(RenderPipeline const &, RenderLightList const &renderLight, CameraInfos const &infos)
	{
		OpenGLState::glEnable(GL_CULL_FACE);
		OpenGLState::glCullFace(GL_BACK);
		OpenGLState::glDepthMask(GL_TRUE);
		OpenGLState::glDepthFunc(GL_LEQUAL);
		OpenGLState::glDisable(GL_BLEND);
		OpenGLState::glDisable(GL_STENCIL_TEST);
		OpenGLState::glEnable(GL_DEPTH_TEST);
		OpenGLState::glClearColor(glm::vec4(0.f, 0.0f, 0.0f, 0.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_programs[PROGRAM_BUFFERING_LIGHT]->use();
		*_programs[PROGRAM_BUFFERING_LIGHT]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[PROGRAM_BUFFERING_LIGHT]->get_resource<Mat4>("view_matrix") = infos.view;

		for (auto &pl : renderLight.pointLight)
		{
			*_programs[PROGRAM_BUFFERING_LIGHT]->get_resource<Mat4>("model_matrix") = infos.view * pl.light.sphereTransform;
			_quadPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING_LIGHT], Properties(), _quadVertices);
		}
	}

}
