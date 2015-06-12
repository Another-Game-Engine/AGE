#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredToneMapping.hh>

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
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>

#include "Render/GeometryManagement/SimpleGeometry.hh"

#define DEFERRED_SHADING_MERGING_VERTEX "deferred_shading/deferred_shading_tone_mapping.vp"
#define DEFERRED_SHADING_MERGING_FRAG "deferred_shading/deferred_shading_tone_mapping.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_TONE_MAPPING = 0,
		PROGRAM_NBR
	};

	DeferredToneMapping::DeferredToneMapping(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
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

		_programs[PROGRAM_TONE_MAPPING] = std::make_shared<Program>(Program(std::string("basic_3d_render"),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));

		Key<Painter> quadPainterKey;

		GetRenderThread()->getQuadGeometry(_quadVertices, quadPainterKey);
		_quadPainter = _painterManager->get_painter(quadPainterKey);
	}

	void DeferredToneMapping::renderPass(RenderPipeline const &, RenderLightList &, CameraInfos const &)
	{
		SCOPE_profile_gpu_i("DeferredToneMapping pass");
		float avgLogLuminance = 1.0f;
		{
			SCOPE_profile_gpu_i("Get pixel value");
			SCOPE_profile_cpu_i("RenderTimer", "Get pixel value");
			std::vector<glm::vec4> data_light;
			_lightAccuInput->generateMipmaps();
			_lightAccuInput->get(_lightAccuInput->nbrMipMap() - 1, GL_RGBA, GL_FLOAT, data_light);
			std::vector<glm::vec4> data_shiny;
			_shinyAccuInput->generateMipmaps();
			_shinyAccuInput->get(_shinyAccuInput->nbrMipMap() - 1, GL_RGBA, GL_FLOAT, data_shiny);
			avgLogLuminance = ((data_light[0].x + data_light[0].y + data_light[0].z) / 3.0f + (data_shiny[0].x + data_shiny[0].y + data_light[0].z) / 3.0f) / 2.0f;
			avgLogLuminance = avgLogLuminance == 0.f ? 0.f : (0.5f / avgLogLuminance) ;
		}
		SCOPE_profile_cpu_i("RenderTimer", "DeferredToneMapping pass");
		{
			SCOPE_profile_gpu_i("Overhead Pipeline");
			SCOPE_profile_cpu_i("RenderTimer", "Overhead Pipeline");
			_programs[PROGRAM_TONE_MAPPING]->use();
			_programs[PROGRAM_TONE_MAPPING]->get_resource<Sampler2D>("diffuse_map").set(_diffuseInput);
			_programs[PROGRAM_TONE_MAPPING]->get_resource<Vec1>("avg_log_luminance").set(avgLogLuminance);
		}

		OpenGLState::glDisable(GL_BLEND);
		OpenGLState::glDisable(GL_CULL_FACE);
		OpenGLState::glDisable(GL_DEPTH_TEST);
		OpenGLState::glDisable(GL_STENCIL_TEST);
		_quadPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_TONE_MAPPING], Properties(), _quadVertices);
	}

}
