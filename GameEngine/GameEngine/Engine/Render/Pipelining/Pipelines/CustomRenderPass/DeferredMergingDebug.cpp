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

#define DEFERRED_SHADING_MERGING_VERTEX "../../Shaders/deferred_shading/deferred_shading_merge_debug.vp"
#define DEFERRED_SHADING_MERGING_FRAG "../../Shaders/deferred_shading/deferred_shading_merge_debug.fp"

#define DEFERRED_SHADING_MERGING_VERTEX "../../Shaders/deferred_shading/deferred_shading_merge_debug.vp"
#define DEFERRED_SHADING_MERGING_FRAG "../../Shaders/deferred_shading/deferred_shading_merge_debug.fp"


namespace AGE
{
	enum Programs
	{
		PROGRAM_MERGING = 0,
//		PROGRAM_DRAW_2DLINE,
		PROGRAM_NBR
	};

	DeferredMergingDebug::DeferredMergingDebug(std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> debugLightRender) :
		ScreenRender(painterManager)
	{
		_debugLightRender = debugLightRender;
		_programs.resize(PROGRAM_NBR);

		_programs[PROGRAM_MERGING] = std::make_shared<Program>(Program(std::string("basic_3d_render"),
		{
			std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_VERTEX, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_FRAG, GL_FRAGMENT_SHADER)
		}));

		Key<Painter> quadPainterKey;

		GetRenderThread()->getQuadGeometry(_quadVertices, quadPainterKey);
		_quadPainter = _painterManager->get_painter(quadPainterKey);
	}

	void DeferredMergingDebug::renderPass(RenderPipeline const &, RenderLightList const &, CameraInfos const &)
	{
		_programs[PROGRAM_MERGING]->use();
		*_programs[PROGRAM_MERGING]->get_resource<Sampler2D>("debug_light_map") = _debugLightRender;

		OpenGLState::glEnable(GL_BLEND);
		OpenGLState::glEnable(GL_DEPTH_TEST);
		OpenGLState::glDisable(GL_CULL_FACE);
		OpenGLState::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		_quadPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_MERGING], Properties(), _quadVertices);
	}

}
