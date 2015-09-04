#include <Render/Pipelining/Pipelines/CustomRenderPass/DepthOfField.hh>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Culling/Output/RenderPipeline.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <Render/ProgramResources/Types/Uniform/Vec2.hh>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>

// TODO TO REMOVE
#include <glm/gtc/matrix_transform.hpp>

#include "Graphic/DRBCameraDrawableList.hpp"
#include "Graphic/DRBPointLightData.hpp"

#define PROGRAM_DOF_VERTEX "deferred_shading/depth_of_field.vp"
#define PROGRAM_DOF_FRAG "deferred_shading/depth_of_field.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_DEPTH_OF_FIELD = 0,
		PROGRAM_NBR
	};

	DepthOfField::DepthOfField(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> depth,
		std::shared_ptr<Texture2D> blurred,
		std::shared_ptr<Texture2D> clean,
		std::shared_ptr<Texture2D> dst) :
		FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
		push_storage_output(GL_COLOR_ATTACHMENT0, dst);

		_depth = depth;
		_clean = clean;
		_blurred = blurred;

		_programs.resize(PROGRAM_NBR);

		auto confManager = GetEngine()->getInstance<ConfigurationManager>();

		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");

		// you have to set shader directory in configuration path
		AGE_ASSERT(shaderPath != nullptr);

		auto vertexShaderPath = shaderPath->getValue() + PROGRAM_DOF_VERTEX;
		auto fragmentShaderPath = shaderPath->getValue() + PROGRAM_DOF_FRAG;

		_programs[PROGRAM_DEPTH_OF_FIELD] = std::make_shared<Program>(Program(std::string("depth_of_field"),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));
		Key<Painter> quadPainterKey;

		GetRenderThread()->getQuadGeometry(_quadVertices, quadPainterKey);
		_quadPainter = _painterManager->get_painter(quadPainterKey);
	}

	void DepthOfField::renderPass(const DRBCameraDrawableList &infos)
	{
		SCOPE_profile_gpu_i("Depth of field");
		SCOPE_profile_cpu_i("RenderTimer", "Depth of field");

		OpenGLState::glDepthMask(GL_FALSE);
		OpenGLState::glDisable(GL_DEPTH_TEST);
		OpenGLState::glDisable(GL_STENCIL_TEST);
		OpenGLState::glDisable(GL_CULL_FACE);

		_programs[PROGRAM_DEPTH_OF_FIELD]->use();
		_programs[PROGRAM_DEPTH_OF_FIELD]->get_resource<Sampler2D>("cleanMap").set(_clean);
		_programs[PROGRAM_DEPTH_OF_FIELD]->get_resource<Sampler2D>("depthMap").set(_depth);
		_programs[PROGRAM_DEPTH_OF_FIELD]->get_resource<Sampler2D>("blurredMap").set(_blurred);

		_quadPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_DEPTH_OF_FIELD], Properties(), _quadVertices);
	}
}
