#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredMergingDebug.hh>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <SpacePartitioning/Ouptut/RenderLight.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <SpacePartitioning/Ouptut/RenderPainter.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <Render/ProgramResources/Types/Uniform/Vec3.hh>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>

#define DEFERRED_SHADING_MERGING_VERTEX "../../Shaders/deferred_shading/deferred_shading_merge_debug.vp"
#define DEFERRED_SHADING_MERGING_FRAG "../../Shaders/deferred_shading/deferred_shading_merge_debug.fp"

#define DRAW_2D_LINE_VERTEX "deferred_shading/draw2DLine.vp"
#define DRAW_2D_LINE_FRAG "deferred_shading/draw2DLine.fp"


namespace AGE
{
	enum Programs
	{
		PROGRAM_MERGING = 0,
		PROGRAM_DRAW_LINE,
		PROGRAM_NBR
	};

	DeferredMergingDebug::DeferredMergingDebug(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> debugLightRender,
		std::shared_ptr<Texture2D> diffuse) :
		FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
		_debugLightRender = debugLightRender;
		_programs.resize(PROGRAM_NBR);

		push_storage_output(GL_COLOR_ATTACHMENT0, diffuse);

		auto confManager = GetEngine()->getInstance<ConfigurationManager>();

		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");

		_programs[PROGRAM_MERGING] = std::make_shared<Program>(Program(std::string("basic_3d_render"),
		{
			std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_VERTEX, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_FRAG, GL_FRAGMENT_SHADER)
		}));

		auto vertexDrawLine = shaderPath->getValue() + DRAW_2D_LINE_VERTEX;
		auto fragDrawLine = shaderPath->getValue() + DRAW_2D_LINE_FRAG;

		_programs[PROGRAM_DRAW_LINE] = std::make_shared<Program>(Program(std::string("draw2DLine"),
		{
			std::make_shared<UnitProg>(vertexDrawLine, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragDrawLine, GL_FRAGMENT_SHADER)
		}));

		Key<Painter> quadPainterKey;

		GetRenderThread()->getQuadGeometry(_quadVertices, quadPainterKey);
		_quadPainter = _painterManager->get_painter(quadPainterKey);
	}

	void DeferredMergingDebug::renderPass(RenderPipeline const &, RenderLightList &, CameraInfos const &)
	{
		_programs[PROGRAM_MERGING]->use();
		_programs[PROGRAM_MERGING]->get_resource<Sampler2D>("debug_light_map").set(_debugLightRender);

		OpenGLState::glEnable(GL_BLEND);
		OpenGLState::glDisable(GL_DEPTH_TEST);
		OpenGLState::glDisable(GL_CULL_FACE);
		OpenGLState::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		_quadPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_MERGING], Properties(), _quadVertices);
	
		OpenGLState::glDisable(GL_DEPTH_TEST);
		OpenGLState::glDisable(GL_STENCIL_TEST);
		OpenGLState::glDepthMask(GL_FALSE);
		_programs[PROGRAM_DRAW_LINE]->use();

		_lines = _painterManager->get_painter(GetRenderThread()->debug2Dlines.painterKey);
		_lines->uniqueDraw(GL_LINES, _programs[PROGRAM_DRAW_LINE], Properties(), GetRenderThread()->debug2Dlines.verticesKey);
	}

}
