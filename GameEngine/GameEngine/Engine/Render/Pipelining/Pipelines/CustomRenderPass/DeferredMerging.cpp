#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredMerging.hh>

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

#define DEFERRED_SHADING_MERGING_VERTEX "../../Shaders/deferred_shading/deferred_shading_merge.vp"
#define DEFERRED_SHADING_MERGING_FRAG "../../Shaders/deferred_shading/deferred_shading_merge.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_MERGING = 0,
		PROGRAM_NBR
	};

	DeferredMerging::DeferredMerging(std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> diffuse,
		std::shared_ptr<Texture2D> specular,
		std::shared_ptr<Texture2D> lightAccumulation) :
					ScreenRender(painterManager)
	{
		_diffuseInput = diffuse;
		_specularInput = specular;
		_lightAccuInput = lightAccumulation;

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

	void DeferredMerging::renderPass(RenderPipeline const &, RenderLightList const &, CameraInfos const &)
	{
		_programs[PROGRAM_MERGING]->use();
		*_programs[PROGRAM_MERGING]->get_resource<Sampler2D>("diffuse_map") = _diffuseInput;
		*_programs[PROGRAM_MERGING]->get_resource<Sampler2D>("light_buffer") = _lightAccuInput;

		OpenGLState::glDisable(GL_BLEND);
		OpenGLState::glDisable(GL_CULL_FACE);
		OpenGLState::glEnable(GL_DEPTH_TEST);
		OpenGLState::glEnable(GL_STENCIL_TEST);
		_quadPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_MERGING], Properties(), _quadVertices);
	}

}
