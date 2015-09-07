#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredBlur.hh>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Culling/Output/RenderPipeline.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <Render/ProgramResources/Types/Uniform/Vec3.hh>
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>

#include "Render/GeometryManagement/SimpleGeometry.hh"

#define BLUR_HORIZONTAL_VERTEX "deferred_shading/deferred_shading_merge.vp"
#define BLUR_HORIZONTAL_FRAG "deferred_shading/blur_horizontal.glsl"
#define BLUR_VERTICAL_VERTEX "deferred_shading/deferred_shading_merge.vp"
#define BLUR_VERTICAL_FRAG "deferred_shading/blur_vertical.glsl"

namespace AGE
{
	enum Programs
	{
		PROGRAM_BLUR_HORIZONTAL = 0,
        PROGRAM_BLUR_VERTICAL,
		PROGRAM_NBR
	};

	DeferredBlur::DeferredBlur(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> diffuse,
        std::shared_ptr<Texture2D> blur) :
					FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
        _blurInput = blur;
		push_storage_output(GL_COLOR_ATTACHMENT0, diffuse);
		_programs.resize(PROGRAM_NBR);

		auto confManager = GetEngine()->getInstance<ConfigurationManager>();
		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");
		// you have to set shader directory in configuration path
		AGE_ASSERT(shaderPath != nullptr);

		auto vertexShaderPathVertical = shaderPath->getValue() + BLUR_VERTICAL_VERTEX;
		auto fragmentShaderPathVertical = shaderPath->getValue() + BLUR_VERTICAL_FRAG;
		_programs[PROGRAM_BLUR_VERTICAL] = std::make_shared<Program>(Program(std::string("basic_3d_render"),
		{
			std::make_shared<UnitProg>(fragmentShaderPathVertical, GL_FRAGMENT_SHADER),
			std::make_shared<UnitProg>(vertexShaderPathVertical, GL_VERTEX_SHADER)
		}));

        auto vertexShaderPathHorizontal = shaderPath->getValue() + BLUR_HORIZONTAL_VERTEX;
		auto fragmentShaderPathHorizontal = shaderPath->getValue() + BLUR_HORIZONTAL_FRAG;
		_programs[PROGRAM_BLUR_HORIZONTAL] = std::make_shared<Program>(Program(std::string("blur_horizontal"),
		{
			std::make_shared<UnitProg>(fragmentShaderPathHorizontal, GL_FRAGMENT_SHADER),
			std::make_shared<UnitProg>(vertexShaderPathHorizontal, GL_VERTEX_SHADER)
		}));

		Key<Painter> quadPainterKey;

		GetRenderThread()->getQuadGeometry(_quadVertices, quadPainterKey);
		_quadPainter = _painterManager->get_painter(quadPainterKey);
	}

	void DeferredBlur::renderPass(const DRBCameraDrawableList &infos)
	{
		OpenGLState::glEnable(GL_BLEND);
		OpenGLState::glBlendFunc(GL_ONE, GL_ZERO);
		OpenGLState::glDisable(GL_CULL_FACE);
        OpenGLState::glDisable(GL_DEPTH_TEST);
        OpenGLState::glDisable(GL_STENCIL_TEST);
		SCOPE_profile_gpu_i("DeferredBlur() first pass of blur horizontal");
		SCOPE_profile_cpu_i("RenderTimer", "DeferredBlur() pass");
		{
			SCOPE_profile_gpu_i("Overhead Pipeline");
			SCOPE_profile_cpu_i("RenderTimer", "Overhead Pipeline");
			_programs[PROGRAM_BLUR_HORIZONTAL]->use();
			_programs[PROGRAM_BLUR_HORIZONTAL]->get_resource<Vec1>("diameter").set(0.05f);
			_programs[PROGRAM_BLUR_HORIZONTAL]->get_resource<Sampler2D>("diffuse_map").set(_blurInput);
			_quadPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_BLUR_HORIZONTAL], Properties(), _quadVertices);
		}
        SCOPE_profile_gpu_i("DeferredBlur() first pass of blur horizontal");
        SCOPE_profile_cpu_i("RenderTimer", "DeferredBlur() pass");
        {
            SCOPE_profile_gpu_i("Overhead Pipeline");
            SCOPE_profile_cpu_i("RenderTimer", "Overhead Pipeline");
           // _programs[PROGRAM_BLUR_VERTICAL]->use();
            //_programs[PROGRAM_MERGING]->get_resource<Sampler2D>("diffuse_map").set(_diffuseInput);
            //_programs[PROGRAM_MERGING]->get_resource<Sampler2D>("light_buffer").set(_lightAccuInput);
            //_programs[PROGRAM_MERGING]->get_resource<Sampler2D>("shiny_buffer").set(_shinyAccuInput);
            //_programs[PROGRAM_MERGING]->get_resource<Vec3>("ambient_color").set(_ambientColor);
			//_quadPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_BLUR_VERTICAL], Properties(), _quadVertices);
        }
	}

}
