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
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>

#include "Render/GeometryManagement/SimpleGeometry.hh"

#define DEFERRED_SHADING_MERGING_VERTEX "deferred_shading/deferred_shading_merge.vp"
#define DEFERRED_SHADING_MERGING_FRAG "deferred_shading/deferred_shading_merge.fp"

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

		auto confManager = GetEngine()->getInstance<ConfigurationManager>();

		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");

		// you have to set shader directory in configuration path
		AGE_ASSERT(shaderPath != nullptr);

		auto vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_MERGING_VERTEX;
		auto fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_MERGING_FRAG;

		_programs[PROGRAM_MERGING] = std::make_shared<Program>(Program(std::string("basic_3d_render"),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
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

	void DeferredMerging::setAmbient(glm::vec3 const &ambient)
	{
		_ambientColor = ambient;
	}

	void DeferredMerging::renderPass(RenderPipeline const &, RenderLightList const &, CameraInfos const &)
	{
		_programs[PROGRAM_MERGING]->use();
		*_programs[PROGRAM_MERGING]->get_resource<Sampler2D>("diffuse_map") = _diffuseInput;
		*_programs[PROGRAM_MERGING]->get_resource<Sampler2D>("light_buffer") = _lightAccuInput;
		*_programs[PROGRAM_MERGING]->get_resource<Vec3>("ambient_color") = _ambientColor;

		OpenGLState::glDisable(GL_BLEND);
		OpenGLState::glDisable(GL_CULL_FACE);
		OpenGLState::glEnable(GL_DEPTH_TEST);
		OpenGLState::glEnable(GL_STENCIL_TEST);
		_quadPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_MERGING], Properties(), _quadVertices);

		OpenGLState::glDisable(GL_DEPTH_TEST);
		OpenGLState::glDisable(GL_STENCIL_TEST);
		OpenGLState::glDepthMask(GL_FALSE);
		_programs[PROGRAM_DRAW_LINE]->use();

		_lines = _painterManager->get_painter(GetRenderThread()->debug2Dlines.painterKey);
		_lines->uniqueDraw(GL_LINES, _programs[PROGRAM_DRAW_LINE], Properties(), GetRenderThread()->debug2Dlines.verticesKey);
	}

}
