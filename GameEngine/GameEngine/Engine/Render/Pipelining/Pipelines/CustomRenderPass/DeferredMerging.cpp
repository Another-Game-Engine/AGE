#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredMerging.hh>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Culling/Output/RenderPipeline.hh>
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

namespace AGE
{
	enum Programs
	{
		PROGRAM_MERGING = 0,
		PROGRAM_NBR
	};

	DeferredMerging::DeferredMerging(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> diffuse,
		std::shared_ptr<Texture2D> lightAccumulation,
		std::shared_ptr<Texture2D> shinyAccumulation) :
					FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
 		_diffuseInput = diffuse;
		_lightAccuInput = lightAccumulation;
		_shinyAccuInput = shinyAccumulation;

		push_storage_output(GL_COLOR_ATTACHMENT0, diffuse);

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

		Key<Painter> quadPainterKey;

		GetRenderThread()->getQuadGeometry(_quadVertices, quadPainterKey);
		_quadPainter = _painterManager->get_painter(quadPainterKey);
	}

	void DeferredMerging::setAmbient(glm::vec3 const &ambient)
	{
		_ambientColor = ambient;
	}

	void DeferredMerging::renderPass(const DRBCameraDrawableList &infos)
	{
		SCOPE_profile_gpu_i("DefferedMerging pass");
		SCOPE_profile_cpu_i("RenderTimer", "DefferedMerging pass");
		{
			SCOPE_profile_gpu_i("Overhead Pipeline");
			SCOPE_profile_cpu_i("RenderTimer", "Overhead Pipeline");
			_programs[PROGRAM_MERGING]->use();
			_programs[PROGRAM_MERGING]->get_resource<Sampler2D>("diffuse_map").set(_diffuseInput);
			_programs[PROGRAM_MERGING]->get_resource<Sampler2D>("light_buffer").set(_lightAccuInput);
			_programs[PROGRAM_MERGING]->get_resource<Sampler2D>("shiny_buffer").set(_shinyAccuInput);
			_programs[PROGRAM_MERGING]->get_resource<Vec3>("ambient_color").set(_ambientColor);
		}
		OpenGLState::glDisable(GL_BLEND);
		OpenGLState::glDisable(GL_CULL_FACE);
		OpenGLState::glDisable(GL_DEPTH_TEST);
		OpenGLState::glDisable(GL_STENCIL_TEST);
		_quadPainter->uniqueDrawBegin(_programs[PROGRAM_MERGING]);
		_quadPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_MERGING], Properties(), _quadVertices);
		_quadPainter->uniqueDrawEnd();
	}

}
